/**
 * DeserializedMaker.cpp
 *
 * @date 2013/02/09
 * @author katsumata
 */

#include "DeserializedMaker.hpp"

#include <stdexcept>
#include <fstream>
#include <iostream>

#include <boost/any.hpp>

#include "../Space.hpp"
#include "../Strategy.hpp"
#include "../BoostAnyConverter.hpp"

#include "../msgpack/SerializedSpace.hpp"

#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"
#include "../../param/RandomParameter.hpp"
#include "../../param/RuntimeParameter.hpp"

#include "../../param/GenerateTopology.hpp"
#include "../../topology/Topology.hpp"
#include "../../rule/SpdRule.hpp"

namespace spd {
namespace core {


/**
 * シリアライズを指定して初期化
 * @param[in, out] param パラメタ
 * @param[in] filename シリアライズ結果
 */
DeserializedMaker::DeserializedMaker(spd::param::Parameter& param, std::string filename) :
			param(param), mpacFile(filename) {
	memoryForConnection = param.getInitialParameter()->getMemory();

}

/**
 * プレイヤを作成し、接続と近傍設定を行う
 * @return 全てのプレイヤ
 */
AllPlayer DeserializedMaker::makePlayers() {

	AllPlayer allPlayer = std::vector<std::shared_ptr<Player>>();

	std::ifstream mpacFile (this->mpacFile);

	std::istreambuf_iterator<char> first(mpacFile);
	std::istreambuf_iterator<char> last;

	const std::string data(first, last);

	try {
		msgpack::unpacked msg;
		msgpack::unpack(&msg, data.data(), data.size());
		msgpack::object obj = msg.get();

		auto ss = obj.as<spd::core::serialize::SerializedSpace>();

		int playerNum = ss.getLinks().size();

		param.getInitialParameter()->setPlayerNum(playerNum);
		// プレイヤのが作成できるかどうか
		long int availableMemory = param.getInitialParameter()->getMemory() -
				(sizeof(std::vector<std::shared_ptr<Player>>) +
						(sizeof(std::shared_ptr<Player>) + sizeof(Player)) * playerNum);
		if (availableMemory < 0) {
			// メモリが少ない場合
			std::cerr << "This program could not construct players due to insufficient memory.\n"
					"Please input a sufficient available memory size or run on other machines.\n\n"
					"At least more than " << std::abs(availableMemory) <<
					" byte(s) of memory is needed for only construct players.\n";
			std::exit(EXIT_FAILURE);
		}
		param.getInitialParameter()->setMemory(availableMemory);
		// この時点のメモリを保存
		memoryForConnection = availableMemory;

		allPlayer.reserve(playerNum);
		for (int id = 0; id < playerNum; ++id) {
			allPlayer.push_back(std::make_shared<Player>(id));
		}

		// トポロジ情報
		spd::param::GenerateTopology tg;
		auto topology = tg.generate(ss.getParam().getTopology());
		param.getNeighborhoodParameter()->setTopology(topology);

		// 乱数情報
		unsigned long long generated = ss.getParam().getGeneratedRand();
		param.getRandomParameter()->getEngine().discard(generated);
		param.getRandomParameter()->addGenerated(generated);

	} catch (msgpack::unpack_error&) {
		std::cout << "unpack failed" << std::endl;
		exit(EXIT_FAILURE);
	} catch (msgpack::type_error& e) {
		std::cout << e.what() << std::endl;
		std::cout << "type error" << std::endl;
		exit(EXIT_FAILURE);
	}

	loadConnectionFromMpac(allPlayer);

	return allPlayer;
}

/**
 * プレイヤの初期化を行う
 * @param[in] allPlayer すべてのプレイヤ
 * @param[in, out] space 盤面
 * @throw std::runtime_error 戦略の設定が出来ない場合
 */
void DeserializedMaker::initPlayer(const AllPlayer& allPlayer, spd::core::Space& space) {

	std::ifstream mpacFile (this->mpacFile);

	std::istreambuf_iterator<char> first(mpacFile);
	std::istreambuf_iterator<char> last;

	const std::string data(first, last);

	try {
		msgpack::unpacked msg;
		msgpack::unpack(&msg, data.data(), data.size());
		msgpack::object obj = msg.get();

		auto ss = obj.as<spd::core::serialize::SerializedSpace>();

		// 戦略リストの初期化
		auto sl = ss.getParam().getStrategyList();
		param.clearList();

		for (int id = 0, size = sl.size(); id < size; ++id) {
			int index = findStrategyById(id, sl);
			if (index < 0) {
				std::cerr << "Could not rebuild a strategy from mpac file." << std::endl;
				exit(EXIT_FAILURE);
			}
			param.addStrategy(
					std::make_pair(
							std::make_shared<Strategy>(sl.at(index).getShortStrategy().c_str()),
							1
					));
		}

		// step 情報
		int step = ss.getStep();
		space.setStep(step);
		if (step != 0) {
			space.setSkipBeforeRules(true);
		}

		// 変わる可能性のあるパラメタ
		param.getRuntimeParameter()->setPayoffP(ss.getParam().getPayoffP());
		param.getRuntimeParameter()->setPayoffR(ss.getParam().getPayoffR());
		param.getRuntimeParameter()->setPayoffT(ss.getParam().getPayoffT());
		param.getRuntimeParameter()->setPayoffS(ss.getParam().getPayoffS());
		param.getRuntimeParameter()->setSelfInteraction(ss.getParam().isSelfInteraction());
		param.getRuntimeParameter()->setStrategyUpdateCycle(ss.getParam().getStrategyUpdateCycle());

		auto& playerMaps = ss.getPlayersMap();
		for (auto pm : playerMaps) {
			for (auto id : pm.second) {
				restorePlayer(allPlayer.at(id), pm.first);
			}
		}

	} catch (msgpack::unpack_error&) {
		std::cout << "unpack failed" << std::endl;
		exit(EXIT_FAILURE);
	} catch (msgpack::type_error& e) {
		std::cout << e.what() << std::endl;
		std::cout << "type error" << std::endl;
		exit(EXIT_FAILURE);
	}
}

// シリアライズから復元
void DeserializedMaker::restorePlayer(
		const std::shared_ptr<Player>& player, const serialize::SerializedPlayer& sp) {

	player->setPreAction(static_cast<Action>(sp.getPreAction()));
	player->setAction(static_cast<Action>(sp.getAction()));

	player->setPreScore(sp.getPreScore());
	player->setScore(sp.getScore());

	player->setPreStrategy(param.getStrategyList().at(sp.getPreStrategyId()).first);
	player->setStrategy(param.getStrategyList().at(sp.getStrategyId()).first);

	// プロパティ
	player->clearProperty();
	for (const serialize::SerializedProperty& prop : sp.getProperties()) {
		boost::any val = converter::stringToBoostAny(prop.getValType(), prop.getVal());
		auto outputType = static_cast<Property::OutputType>(prop.getOutputType());
		player->addProperty(Property(prop.getName(), val, outputType));
	}
}

// 戦略ID からシリアライズした戦略を探す
int DeserializedMaker::findStrategyById(int id, std::vector<serialize::SerializedStrategy>& strategies) {

	for (int i = 0, size = strategies.size(); i < size; ++i) {
		if (strategies.at(i).getId() == id) {
			return i;
		}
	}

	return -1;
}

/**
 * 構造と近傍の再初期化を行う
 * @param[in, out] allPlayer すべてのプレイヤ
 */
void DeserializedMaker::resetTopology(AllPlayer& allPlayer) {
	loadConnectionFromMpac(allPlayer);
}


// 接続と近傍の設定
void DeserializedMaker::loadConnectionFromMpac(AllPlayer& allPlayer) {

	//全ての接続を削除
	for (auto& player : allPlayer) {
		player->deleteLink();
	}

	std::ifstream mpacFile (this->mpacFile);

	std::istreambuf_iterator<char> first(mpacFile);
	std::istreambuf_iterator<char> last;

	const std::string data(first, last);

	try {
		msgpack::unpacked msg;
		msgpack::unpack(&msg, data.data(), data.size());
		msgpack::object obj = msg.get();

		auto ss = obj.as<spd::core::serialize::SerializedSpace>();

		// プレイヤ数が増えていたら削除
		unsigned int unitNum = ss.getLinks().size();
		if (unitNum < allPlayer.size()) {
			allPlayer.erase(std::begin(allPlayer) + unitNum, std::end(allPlayer));
		}

		// エッジ処理
		int edgeNum = 0;
		auto& edges = ss.getLinks();
		for (auto edge : edges) {
			edgeNum += edge.size();
		}
		auto availableMemory = memoryForConnection -
				static_cast<long int>(sizeof(std::weak_ptr<Player>) * edgeNum);
		if (availableMemory < 0) {
			std::cerr << "Could not connect players due to memory problem.\n"
					"To simulate this setting, add more than " <<
					std::abs(availableMemory) << " byte(s) of memory.\n";
			std::exit(EXIT_FAILURE);
		}
		param.getInitialParameter()->setMemory(availableMemory);

		std::cout << "connecting players." << std::endl;

		// 接続
		for (int i = 0, size = edges.size(); i < size; ++i) {
			for (auto link : edges.at(i)) {
				allPlayer.at(i)->linkTo(allPlayer.at(link));

				allPlayer.at(link)->linkTo(allPlayer.at(i));
			}
		}

		// 近傍の設定
		auto topology = this->param.getNeighborhoodParameter()->getTopology();
		std::cout << "set neighbor players." << std::endl;
		topology->setNeighbors(allPlayer, this->param);

	} catch (msgpack::unpack_error&) {
		std::cout << "unpack failed" << std::endl;
		exit(EXIT_FAILURE);
	} catch (msgpack::type_error& e) {
		std::cout << e.what() << std::endl;
		std::cout << "type error" << std::endl;
		exit(EXIT_FAILURE);
	}
}


} /* namespace core */
} /* namespace spd */
