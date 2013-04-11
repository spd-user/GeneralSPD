/**
 * GEXFBasedMaker.cpp
 *
 * @date 2013/01/28
 * @author katsumata
 */

#include "GEXFBasedMaker.hpp"

#include <stdexcept>
#include <cmath>

#include "../Space.hpp"
#include "../Player.hpp"
#include "../Strategy.hpp"
#include "../Converter.hpp"
#include "../BoostAnyConverter.hpp"

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

using namespace boost::property_tree;

/*
 * GEXFファイルを指定して初期化
 */
GEXFBasedMaker::GEXFBasedMaker(spd::param::Parameter& param, std::string filename) :
				param(param), gexfFile(filename) {
	memoryForConnection = param.getInitialParameter()->getMemory();
}

/*
 * プレイヤを作成し接続と近傍設定を行う
 * @param allPlayers 全てのプレイヤ
 */
AllPlayer GEXFBasedMaker::makePlayers() {

	AllPlayer allPlayer = std::vector<std::shared_ptr<Player>>();

	ptree pt;
	read_xml(this->gexfFile, pt, xml_parser::no_comments);

	try {
		// ノード数
		const auto& nodes = pt.get_child("gexf.graph.nodes");
		int nodeNum = nodes.size();

		param.getInitialParameter()->setPlayerNum(nodeNum);

		// プレイヤの作成
		auto playersMemory = sizeof(std::vector<std::shared_ptr<Player>>) +
				(sizeof(std::shared_ptr<Player>) + sizeof(Player)) * nodeNum;
		long int availableMemory = param.getInitialParameter()->getMemory() - static_cast<long int>(playersMemory);
		if (availableMemory < 0) {
			// メモリが少なく、プレイヤを作れない場合
			std::cerr << "This program could not construct players due to insufficient memory.\n"
					"Please input a sufficient available memory size or run on other machines.\n\n"
					"At least more than " << std::abs(availableMemory) <<
					" byte(s) of memory is needed for only construct players.\n";
			std::exit(EXIT_FAILURE);
		}
		param.getInitialParameter()->setMemory(availableMemory);
		// この時点のメモリを保存
		memoryForConnection = availableMemory;

		allPlayer.reserve(nodeNum);
		for (int id = 0; id < nodeNum; ++id) {
			allPlayer.push_back(std::make_shared<Player>(id));
		}

		// メタデータの解析
		const auto& metaDatas = pt.get_child("gexf.meta");
		// トポロジ情報の取得
		std::string topologyType;
		for (auto meta : metaDatas) {
			auto pos = getKeywordPos(meta, "topology");
			if (pos != std::string::npos) {
				topologyType = meta.second.data().substr(pos);
			}
		}
		// トポロジが分からない場合は終了
		if (topologyType.empty()) {
			throw std::runtime_error("Could not find a schema that is mentioning "
					"topology structure in a GEXF file.");
		}
		spd::param::GenerateTopology topologyGenerator;
		auto topology = topologyGenerator.generate(topologyType);
		// 構造の設定
		this->param.getNeighborhoodParameter()->setTopology(topology);

		// 乱数生成情報の取得
		for (auto meta : metaDatas) {
			auto pos = getKeywordPos(meta, "generated-rand");
			if (pos != std::string::npos) {
				// 乱数の除去
				unsigned long long generated = std::stoull(meta.second.data().substr(pos));
				param.getRandomParameter()->getEngine().discard(generated);
				param.getRandomParameter()->addGenerated(generated);
				break;
			}
		}

	} catch (boost::property_tree::ptree_bad_path& e){
		std::cerr << "Could not create players from gexf file." << std::endl;
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	loadConnectionFromGEXF(allPlayer);

	return allPlayer;
}

/*
 * プレイヤの初期化を行う
 * @param[in, out] players すべてのプレイヤ
 * @param[in, out] space 盤面
 * @throw std::runtime_error 戦略の設定が出来ない場合
 */
void GEXFBasedMaker::initPlayer(const AllPlayer& players, spd::core::Space& space) {

	ptree pt;
	read_xml(this->gexfFile, pt, xml_parser::no_comments);

	try {
		// メタデータの解析
		const auto& metaDatas = pt.get_child("gexf.meta");
		// 戦略情報の取得
		param.clearList();
		for (const ptree::value_type& meta : metaDatas) {
			auto pos = getKeywordPos(meta, "strategy");
			if (pos != std::string::npos) {
				param.addStrategy(
						std::make_pair(
								std::make_shared<Strategy>(meta.second.data().substr(pos).c_str()),
								1));
			}
		}

		// step情報の取得
		int step = -1;
		for (const ptree::value_type& meta : metaDatas) {
			auto pos = getKeywordPos(meta, "step");
			if (pos != std::string::npos) {
				step = std::stoi(meta.second.data().substr(pos));
			}
		}
		if (step < 0) {
			throw std::runtime_error("Could not find a schema that is mentioning a step.");
		} else if (step != 0) {
			// 0ステップでなければ、前ルールは飛ばす
			space.setSkipBeforeRules(true);
			space.setStep(step);
		}
		// 変わるかもしれないパラメタの設定
		for (const ptree::value_type& meta : metaDatas) {
			auto pos = getKeywordPos(meta, "payoff-R");
			if (pos != std::string::npos) {
				param.getRuntimeParameter()->setPayoffP(
						std::stod(meta.second.data().substr(pos)));
			}
			pos = getKeywordPos(meta, "payoff-S");
			if (pos != std::string::npos) {
				param.getRuntimeParameter()->setPayoffS(
						std::stod(meta.second.data().substr(pos)));
			}
			pos = getKeywordPos(meta, "payoff-T");
			if (pos != std::string::npos) {
				param.getRuntimeParameter()->setPayoffT(
						std::stod(meta.second.data().substr(pos)));
			}
			pos = getKeywordPos(meta, "payoff-P");
			if (pos != std::string::npos) {
				param.getRuntimeParameter()->setPayoffP(
						std::stod(meta.second.data().substr(pos)));
			}

			pos = getKeywordPos(meta, "strategy-update-cycle");
			if (pos != std::string::npos) {
				param.getRuntimeParameter()->setStrategyUpdateCycle(
						std::stoi(meta.second.data().substr(pos)));
			}
			pos = getKeywordPos(meta, "self-interaction");
			if (pos != std::string::npos) {
				param.getRuntimeParameter()->setSelfInteraction(
						0 != std::stoi(meta.second.data().substr(pos)));
			}
		}

		// ノードの属性を取得
		std::vector<std::pair<std::string, std::string>> proparty;
		const auto& attrs = pt.get_child("gexf.graph.attributes");
		for (auto attr : attrs) {
			if (std::string(attr.first.data()) == "<xmlattr>") {
				continue;
			}
			proparty.push_back(
					std::make_pair(
							attr.second.get<std::string>("<xmlattr>.title"),
							attr.second.get<std::string>("<xmlattr>.type")));
		}

		// ノードを取得
		for (auto& player : players) {
			player->clearProperty();
		}

		const auto& nodes = pt.get_child("gexf.graph.nodes");
		for (auto node : nodes) {
			auto nodeId = node.second.get<int>("<xmlattr>.id");

			auto& attrs = node.second.get_child("attvalues");
			for(auto attr : attrs) {
				auto attrId = attr.second.get<int>("<xmlattr>.for");
				auto attrVal = attr.second.get<std::string>("<xmlattr>.value");

				setPlayerVal(players.at(nodeId), attrId, proparty, attrVal);
			}
		}

	} catch (boost::property_tree::ptree_bad_path& e){
		std::cerr << "Could not initialize players from gexf file." << std::endl;
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

}

void GEXFBasedMaker::setPlayerVal(
		const std::shared_ptr<Player>& player,
		int attrId,
		const std::vector<std::pair<std::string, std::string>>& prop,
		std::string value) {

	int strategyNum;

	switch (attrId) {
		case 0:
			player->setPreAction(converter::charToActoin(value.at(0)));
			break;
		case 1:
			player->setAction(converter::charToActoin(value.at(0)));
			break;
		case 2:
			player->setPreScore(std::stod(value));
			break;
		case 3:
			player->setScore(std::stod(value));
			break;
		case 4:
			strategyNum = getStrategyNum(value);
			if (strategyNum < 0) {
				std::string errMsg ("Could not restore pre-strategy from "
						+ value + "(player id : " + std::to_string(player->getId()) + ")");
				throw std::invalid_argument(errMsg);
			}
			player->setPreStrategy(param.getStrategyList().at(strategyNum).first);
			break;
		case 5:
			strategyNum = getStrategyNum(value);
			if (strategyNum < 0) {
				std::string errMsg ("Could not restore strategy from "
						+ value + "(player id : " + std::to_string(player->getId()) + ")");
				throw std::invalid_argument(errMsg);
			}
			player->setStrategy(param.getStrategyList().at(strategyNum).first);
			break;
		default:

			auto outputType = static_cast<Property::OutputType>(value[value.size() -1]);
			boost::any pValue = converter::stringToBoostAny(prop.at(attrId).second, value.substr(0, value.size() - 1));

			player->addProperty(Property(prop.at(attrId).first, pValue, outputType));

			break;
	}
}

int GEXFBasedMaker::getStrategyNum(std::string strategy) {

	auto strategyList = param.getStrategyList();

	for (int i = 0, size = strategyList.size(); i < size; ++i) {
		if (strategyList.at(i).first->getShortStrategy() == strategy) {
			return i;
		}
	}
	return -1;
}

/*
 * 構造と近傍の再初期化を行う
 * @param[in, out] players すべてのプレイヤ
 */
void GEXFBasedMaker::resetTopology(AllPlayer& allPlayer) {
	loadConnectionFromGEXF(allPlayer);
}

// 接続と近傍の設定
void GEXFBasedMaker::loadConnectionFromGEXF(AllPlayer& allPlayer) {

	// すべての接続を削除
	for (auto player : allPlayer) {
		player->deleteLink();
	}

	ptree pt;
	read_xml(this->gexfFile, pt, xml_parser::no_comments);

	try {
		// 追加ノードがあった場合は削除
		const auto& nodes = pt.get_child("gexf.graph.nodes");
		unsigned int nodeNum = nodes.size();
		if (nodeNum < allPlayer.size()) {
			allPlayer.erase(std::begin(allPlayer) + nodeNum, std::end(allPlayer));
		}

		// edges を読み込み、接続の作成を行う
		const auto& edges = pt.get_child("gexf.graph.edges");
		// edge用
		auto availableMemory = memoryForConnection -
				static_cast<long int>(sizeof(std::weak_ptr<Player>) * edges.size() * 2);
		if (availableMemory < 0) {
			std::cerr << "Could not connect players due to memory problem.\n"
					"To simulate this setting, add more than " <<
					std::abs(availableMemory) << " byte(s) of memory.\n";
			std::exit(EXIT_FAILURE);
		}
		param.getInitialParameter()->setMemory(availableMemory);

		std::cout << "connecting players." << std::endl;
		for (auto edge : edges) {
			allPlayer.at(edge.second.get<int>("<xmlattr>.source"))->
					linkTo(allPlayer.at(edge.second.get<int>("<xmlattr>.target")));

			allPlayer.at(edge.second.get<int>("<xmlattr>.target"))->
					linkTo(allPlayer.at(edge.second.get<int>("<xmlattr>.source")));
		}

		// 近傍の設定
		auto topology = this->param.getNeighborhoodParameter()->getTopology();
		std::cout << "set neighbor players." << std::endl;
		topology->setNeighbors(allPlayer, this->param);

	} catch (boost::property_tree::ptree_bad_path& e){
		std::cerr << "File format error." << std::endl;
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
 * meta子ツリーの各要素から、keyword 要素の propType= の要素部分の開始位置を取得する。
 * ない場合は、string::npos// load connections and set neighbors
	void loadConnectionFromGEXF(AllPlayer& allPlayer);
 */
std::string::size_type GEXFBasedMaker::getKeywordPos(const ptree::value_type& meta, std::string propType) {

	if ((meta.first == "keywords") &&
			(meta.second.data().find(propType) != std::string::npos)) {

		return propType.size() + 1; // 最後の = の分を加える
	}

	return std::string::npos;
}

} /* namespace core */
} /* namespace spd */
