/**
 * MembraneDetectRule.cpp
 *
 * @date 2014/01/24
 * @author katsumata
 */
#include "MembraneDetectRule.hpp"

#include <climits>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <thread>

#include "../../core/Player.hpp"
#include "../../core/Strategy.hpp"
#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"

#include "countingRule/MembranePropCount.hpp"

namespace spd {
namespace rule {

/*
 * プロパティの初期化
 */
void MembraneDetectRule::initialize(
				const std::shared_ptr<Player>& player,
				const AllPlayer& allPlayers,
				const spd::param::Parameter& param) {

	// 初期化
	initProp(player);
}


/*
 * 検知
 */
void MembraneDetectRule::runRule(
		const std::shared_ptr<Player>& frontPlayer,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step) {

	NeighborhoodType neiType = NeighborhoodType::GAME;


	bool notStable;

	// 初めだけ
	if (frontPlayer->getId() != 0) {
		return;
	}

	// 初期化
	for (auto& player : allPlayers) {
		initProp(player);
	}

	int core = param.getCore();

	// 1コアが担当するプレイヤ数
	int playerNum = allPlayers.size();
	int breadth = playerNum / core;

	std::vector<std::thread> thr(core);

	for (int i = 0, size = thr.size(); i < size; ++i) {
		thr[i] = std::thread(
				[&, i]{

			int from = breadth * i;
			int to = (i + 1 < core) ? breadth * (i + 1) : playerNum;

			auto filter = filtering(param);

			for (int id = from; id < to; ++id) {
				grouping(allPlayers[id], neiType, filter);
			}
		}
		);
	}
	for (std::thread& t : thr) {
		t.join();
	}

	// 検知開始
	std::vector<bool> changes(core);
	do {

		// 調べる(1ステップ)
		for (int i = 0, size = thr.size(); i < size; ++i) {
			thr[i] = std::thread(
					[&, i]{

				int from = breadth * i;
				int to = (i + 1 < core) ? breadth * (i + 1) : playerNum;

				for (int id = from; id < to; ++id) {
					spreadMembraneDetect(allPlayers[id], neiType);
				}
			}
			);
		}
		for (std::thread& t : thr) {
			t.join();
		}

		// アップデート
		for (int i = 0, size = thr.size(); i < size; ++i) {
			thr[i] = std::thread(
					[&, i]{
				bool result = false;

				int from = breadth * i;
				int to = (i + 1 < core) ? breadth * (i + 1) : playerNum;

				for (int id = from; id < to; ++id) {
					result = changesStatus(allPlayers[id]) || result;
				}
				changes[i] = result;
			}
			);
		}
		notStable = false;
		for (int i = 0, size = thr.size(); i < size; ++i) {
			thr[i].join();
			notStable = changes[i] || notStable;
		}
	} while (notStable);

	// 最終処理
	for (int i = 0, size = thr.size(); i < size; ++i) {
		thr[i] = std::thread(
				[&, i]{

			int from = breadth * i;
			int to = (i + 1 < core) ? breadth * (i + 1) : playerNum;

			for (int id = from; id < to; ++id) {
				postHandling(allPlayers[id], neiType);
			}
		}
		);
	}
	for (std::thread& t : thr) {
		t.join();
	}
	// 最終結果を合算
	for (int i = 0, size = thr.size(); i < size; ++i) {
		thr[i] = std::thread(
				[&, i]{
			bool result = false;

			int from = breadth * i;
			int to = (i + 1 < core) ? breadth * (i + 1) : playerNum;

			for (int id = from; id < to; ++id) {
				changesStatus(allPlayers[id]) || result;
			}
		}
		);
	}
	for (int i = 0, size = thr.size(); i < size; ++i) {
		thr[i].join();
	}

}

std::vector<bool> MembraneDetectRule::filtering(const spd::param::Parameter& param){

	auto strategies = param.getStrategyList();
	// それが膜になり得るかどうかのフィルタ
	std::vector<bool> filter(strategies.size() * 2, false);

	// それが存在するかどうかの可能性
	std::vector<bool> potential(strategies.size() * 2, false);
	for (int i = 0, size = strategies.size(); i < size; ++i) {
		std::string shortStrategy = (strategies[i].first)->getShortStrategy();

		// C が含まれている
		if (shortStrategy.find("C") != std::string::npos) {
			potential[2 * i] = true;
		}

		// D が含まれている
		if (shortStrategy.find("D") != std::string::npos) {
			potential[2 * i + 1] = true;
		}
	}

	int probabilityC = 0;
	int probabilityD = 0;


	for (int i = 0, size = strategies.size(); i < size; ++i) {
		// C, D 両方あるか
		if (potential[2 * i] && potential[2 * i + 1]) {
			filter[2 * i] = true;
			filter[2 * i + 1] = true;
		}
		// どのぐらい、それぞれがそんざいするか
		if (potential[2 * i]) {
			probabilityC++;
		}
		if (potential[2 * i + 1]) {
			probabilityD++;
		}
	}

	for (int i = 0, size = strategies.size(); i < size; ++i) {

		// 他にCがいないなら、その戦略のCは膜にならない
		if (probabilityC < 2) {
			filter[2 * i] = false;
		}

		if (probabilityD < 2) {
			filter[2 * i + 1] = false;
		}
	}

	return filter;
}

/*
 * 初期のグループ分け
 */
void MembraneDetectRule::grouping(const std::shared_ptr<Player> player,
			NeighborhoodType type,
			std::vector<bool>& filter) {

	auto playerStrategyId = player->getStrategy()->getId();
	auto playerAction = player->getAction();

	auto& neighbors = player->getNeighbors(type);

	// same strategy, same action
	bool ss = false;

	// same strategy, different action
	bool sd = false;

	// different strategy, same action
	bool ds = false;

	// different strategy, different action
	bool dd = false;



	// 自分は考えない
	for (int r = 1, rMax = neighbors->size(); r < rMax; ++r) {

		// 同じ近傍距離のプレイヤ
		auto nNeighbors = neighbors->at(r);

		for (auto& opponentWP : *(nNeighbors)) {
			auto opponent = opponentWP.lock();
			if (opponent == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player (mem detect rule - grouping).");
			}

			if (playerStrategyId == opponent->getStrategy()->getId()) {
				// same strategy
				if (playerAction == opponent->getAction()) {
					// same action
					ss = true;
				} else {
					sd = true;
				}

			} else {
				// different strategy
				if (playerAction == opponent->getAction()) {
					// same action
					ds = true;
				} else {
					dd = true;
				}
			}
		}
	}


	// 場合分け
	int group = -1;
	if (ss && sd && !ds && !dd) {
		group = 1;
	} else if (ss && !sd && ds ) {
		// dd はどっちでもいい
		group = 2;
	} else if (sd && ds) {
		// ss, dd はどっちでもいい
		group = 3;
	} else if (ss && !sd && !ds && !dd) {
		// 同じ組み合わせのプレイヤとだけ
		group = 0;
	}


	// フィルタリング
	int actionInt = 0;
	if (spd::core::converter::actionToChar(playerAction) == 'D') {
		actionInt = 1;
	}
	// 0-4でなく、膜になり得ないプレイヤは飛ばす
	if (!(filter.at(2 * playerStrategyId + actionInt)) && (group != -1)) {
		player->getProperty(PROP_NAMES[0]).setValue(-2);
		player->getProperty(PROP_NAMES[1]).setValue(-2);
		return;
	}

	// 現在と未来を設定
	player->getProperty(PROP_NAMES[0]).setValue(group);
	player->getProperty(PROP_NAMES[1]).setValue(group);
}

/**
 * 膜判定が広がる
 * @param player プレイヤ
 * @param type 考える膜のタイプ
 */
void MembraneDetectRule::spreadMembraneDetect(const std::shared_ptr<Player> player,
		NeighborhoodType type) {

	auto& neighbors = player->getNeighbors(type);

	// グループ番号
	auto thisGroup = player->getProperty(PROP_NAMES[0]).getValueAs<int>();

	switch (thisGroup) {
		case 0:
			groupZeroBehavior(player, neighbors);
			break;

		case 1:
		case 2:
			groupOneTwoBehavior(player, neighbors);
			break;
		default:
			// -1 や 4 がくるけど何もしない
			break;
	}
}


/*
 * グループ0の動き
 * 同戦略同行動とのみ接続
 * @param player プレイヤ
 * @param neighbors 近傍
 */
void MembraneDetectRule::groupZeroBehavior(const std::shared_ptr<Player> player,
		Neighbors& neighbors) {


	// 移動ポイント
	int minMove = INT_MAX;

	// グループ1, 2と接しているか
	bool hasGroup1 = false;
	bool hasGroup2 = false;


	// 自分は考えない
	for (int r = 1, rMax = neighbors->size(); r < rMax; ++r) {

		// 同じ近傍距離のプレイヤ
		auto nNeighbors = neighbors->at(r);

		for (auto& opponentWP : *(nNeighbors)) {
			auto opponent = opponentWP.lock();
			if (opponent == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player (mem detect rule - zero).");
			}

			int oppGroup = opponent->getProperty(PROP_NAMES[0]).getValueAs<int>();

			if ((oppGroup == 1) || (oppGroup == 2)) {
				// 小さいmove point へ
				minMove = std::min(opponent->getProperty(PROP_NAMES[2]).getValueAs<int>(), minMove);
				if (oppGroup == 1) {
					hasGroup1 = true;
				} else {
					hasGroup2 = true;
				}
			}
		}
	}

	// 動きポイントを1 増やす
	minMove++;

	if (hasGroup1 && hasGroup2) {
		// 1, 2がくっついた -> 4 になる
		player->getProperty(PROP_NAMES[1]).setValue(4);
		player->getProperty(PROP_NAMES[3]).setValue(minMove);

	} else if (hasGroup1 || hasGroup2){
		// それぞれが広がる
		int groupVal = (hasGroup1) ? 1 : 2;

		player->getProperty(PROP_NAMES[1]).setValue(groupVal);
		player->getProperty(PROP_NAMES[3]).setValue(minMove);
	}
}

/*
 * グループ1と2の動き
 * @param player プレイヤ
 * @param neighbors 近傍
 */
void MembraneDetectRule::groupOneTwoBehavior(const std::shared_ptr<Player> player,
		Neighbors& neighbors) {

	// 自分の戦略と行動
	auto playerStrategyId = player->getStrategy()->getId();
	auto playerAction = player->getAction();

	// グループ番号
	auto thisGroup = player->getProperty(PROP_NAMES[0]).getValueAs<int>();

	// 対のグループ番号
	auto oppositeGroup = (thisGroup != 1) ? 1 : 2;

	// 次に膜になる
	bool becomesMembrane = false;
	// 移動ポイント
	int minMove = INT_MAX;

	// 自分は考えない
	for (int r = 1, rMax = neighbors->size(); r < rMax; ++r) {
		// 同じ近傍距離のプレイヤ
		auto nNeighbors = neighbors->at(r);
		for (auto& opponentWP : *(nNeighbors)) {
			auto opponent = opponentWP.lock();
			if (opponent == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player (mem detect rule - one,two).");
			}

			// 同戦略同行動からのみ派生
			if ((playerStrategyId == opponent->getStrategy()->getId()) &&
					(playerAction == opponent->getAction())) {

				auto opponentGroup = opponent->getProperty(PROP_NAMES[0]).getValueAs<int>();
				auto opponentMovePoint = opponent->getProperty(PROP_NAMES[2]).getValueAs<int>();

				// グループ4の場合は、移動ポイントが必要
				if ((opponentGroup == 4) && (opponentMovePoint > 0)) {
					// 終わりでよい
					player->getProperty(PROP_NAMES[1]).setValue(4);
					player->getProperty(PROP_NAMES[3]).setValue(opponentMovePoint - 1);
					return;

				} else if (opponentGroup == oppositeGroup) {
					// 相手側グループの場合、現在のポイントと比較
					minMove = std::min(minMove, player->getProperty(PROP_NAMES[2]).getValueAs<int>());
					becomesMembrane = true;

				}
			}
		}
	}

	if (becomesMembrane) {
		player->getProperty(PROP_NAMES[1]).setValue(4);
		player->getProperty(PROP_NAMES[3]).setValue(minMove);
	}

}


/*
 * コピー
 */
bool MembraneDetectRule::changesStatus(const std::shared_ptr<Player> player) {

	if (player->getProperty(PROP_NAMES[0]).getValueAs<int>() ==
			player->getProperty(PROP_NAMES[1]).getValueAs<int>()) {

		return false;
	}
	// 未来を現在へコピーする
	// group id
	player->getProperty(PROP_NAMES[0]).setValue(
			player->getProperty(PROP_NAMES[1]).getValueAs<int>());
	// move point
	player->getProperty(PROP_NAMES[2]).setValue(
			player->getProperty(PROP_NAMES[3]).getValueAs<int>());

	return true;
}

/*
 * 事後処理
 *
 * 膜で無いものについて上書きで、消していく
 */
void MembraneDetectRule::postHandling(const std::shared_ptr<Player> player,
		NeighborhoodType type) {

	auto& neighbors = player->getNeighbors(type);

	// 自分の戦略と行動
	auto playerStrategyId = player->getStrategy()->getId();
	auto playerAction = player->getAction();


	// 自分は考えない
	for (int r = 1, rMax = neighbors->size(); r < rMax; ++r) {
		// 同じ近傍距離のプレイヤ
		auto nNeighbors = neighbors->at(r);

		for (auto& opponentWP : *(nNeighbors)) {
			auto opponent = opponentWP.lock();
			if (opponent == nullptr) {
				// 近傍がいない場合終了
				throw std::runtime_error("Could not find a neighbor of a player (mem detect rule - zero).");
			}

			// 同じ戦略で、異なる行動のプレイヤがどうなっているかで判断
			if ((playerStrategyId == opponent->getStrategy()->getId()) &&
					(playerAction != opponent->getAction()) &&
					(opponent->getProperty(PROP_NAMES[0]).getValueAs<int>() == -1)) {

				player->getProperty(PROP_NAMES[1]).setValue(-1);
				return;
			}
		}
	}
}


/*
 * 初期化
 */
void MembraneDetectRule::initProp(const std::shared_ptr<Player>& player) {

	for (int i = 0, max = PROP_NAMES.size(); i < max; ++i) {
		try {

			auto propName = PROP_NAMES.at(i);

			auto& prop = player->getProperty(propName);

			// 先頭プレイヤであり、"_"で始まらないプロパティ、かつ数え上げが無い場合は設定
			if ((player->getId() == 0) && (propName.front() != '_') &&
					(prop.getCountingMethod() == nullptr)) {
				std::shared_ptr<spd::core::PropertyCounting> counting =
						std::make_shared<spd::rule::counting::MembranePropCount>();

				prop.setCountingMethod(counting);
			}

			prop.setValue(INIT_VALS.at(i));
		} catch (std::invalid_argument& e) {

			auto propName = PROP_NAMES.at(i);


			auto propType = spd::core::Property::OutputType::NOT;
			std::shared_ptr<spd::core::PropertyCounting> counting = nullptr;

			// 先頭プレイヤであり、"_"で始まらないプロパティでは必要なのを設定
			if ((player->getId() == 0) && (propName.front() != '_')) {
				propType = spd::core::Property::OutputType::SPECIAL;
				counting = std::make_shared<spd::rule::counting::MembranePropCount>();
			}

			spd::core::Property p (PROP_NAMES.at(i), INIT_VALS.at(i), propType, counting);
			player->addProperty(p);
		}
	}
}


} /* namespace rule */
} /* namespace spd */
