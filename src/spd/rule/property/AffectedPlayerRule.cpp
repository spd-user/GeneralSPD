/**
 * AffectedPlayer.cpp
 *
 * @date 2014/02/04
 * @author katsumata
 */
#include "AffectedPlayerRule.hpp"

#include <stdexcept>
#include <vector>
#include <thread>

#include "../../core/Player.hpp"
#include "../../core/Strategy.hpp"
#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"

#include "countingRule/PropCount.hpp"

#include "MembraneDetectRule.hpp"

namespace spd {
namespace rule {


/*
 * プロパティを設定する
 * @param[in, out] player 対象プレイヤ
 * @param[in] allPlayers 全てのプレイヤ
 * @param[in] param パラメタ
 */
void AffectedPlayerRule::initialize(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param) {

	initProp(player);
}

/*
 * 影響検知
 * @param[in, out] player 対象プレイヤ
 * @param[in] allPlayers 全てのプレイヤ
 * @param[in] param パラメタ
 * @param[in] step 実行ステップ
 */
void AffectedPlayerRule::runRule(
		const std::shared_ptr<Player>& frontPlayer,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step) {

	NeighborhoodType neiType = NeighborhoodType::GAME;


	bool notStable;

	// 初のプレイヤの時だけ
	if (frontPlayer->getId() != 0) {
		return;
	}

	// 初期化
	for (auto& player : allPlayers) {
		initProp(player);
	}

	auto filter = filtering(allPlayers, param);

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

			for (int id = from; id < to; ++id) {
				directAffect(allPlayers[id], neiType, filter);
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
					spreadAffect(allPlayers[id], neiType);
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

}

/*
 * 直接な影響
 */
void AffectedPlayerRule::directAffect(const std::shared_ptr<Player> player,
			NeighborhoodType type,
			std::vector<bool>& filter) {

	auto playerStrategyId = player->getStrategy()->getId();
	auto playerAction = player->getAction();

	// フィルタリング
	// 逆の行動
	int oppsiteActInt = 0;
	if (spd::core::converter::actionToChar(playerAction) != 'D') {
		oppsiteActInt = 1;
	}
	// 膜の影響を受けることはあり得ないプレイヤは飛ばす
	if (!(filter.at(2 * playerStrategyId + oppsiteActInt))) {
		// 現在と未来を設定
		int iGroup = static_cast<int>(Affect::IGNORE);
		player->getProperty(PROP_NAMES[0]).setValue(iGroup);
		player->getProperty(PROP_NAMES[1]).setValue(iGroup);
		return;
	}

	auto& neighbors = player->getNeighbors(type);

	bool hasEnemy = false;
	bool hasMembrane = false;

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

			if (playerAction != opponent->getAction()) {
				if (playerStrategyId != opponent->getStrategy()->getId()) {
					// 行動も戦略も違う
					hasEnemy = true;
				} else {
					auto propertyVal = static_cast<MembraneDetectRule::Group>(
							opponent->getProperty("MemGroup").getValueAs<int>());

					if (propertyVal == MembraneDetectRule::Group::COMBINE ||
							propertyVal == MembraneDetectRule::Group::BOTH_SIDE) {
						// 行動だけ違う
						hasMembrane = true;
					}
				}
			}
		}
	}
	auto group = Affect::BLANK;
	if (hasEnemy) {
		group = Affect::ENEMY;
	} else if (hasMembrane) {
		group = Affect::MEMBRANE;
	}

	// 現在と未来を設定
	int iGroup = static_cast<int>(group);
	player->getProperty(PROP_NAMES[0]).setValue(iGroup);
	player->getProperty(PROP_NAMES[1]).setValue(iGroup);
}

/*
 * 影響が広がる
 * @param player プレイヤ
 * @param type 考える膜のタイプ
 */
void AffectedPlayerRule::spreadAffect(const std::shared_ptr<Player> player,
		NeighborhoodType type) {

	auto thisGroup = static_cast<Affect>(player->getProperty(PROP_NAMES[0]).getValueAs<int>());

	// 空き以外なら終わり
	if (thisGroup != Affect::BLANK) {
		return;
	}

	// 近傍
	auto& neighbors = player->getNeighbors(type);

	// 行動と戦略
	auto playerStrategyId = player->getStrategy()->getId();
	auto playerAction = player->getAction();

	bool hasEnemy = false;
	bool hasMembrane = false;

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

			// 戦略と行動が同じでなければ飛ばす
			if ((playerStrategyId != opponent->getStrategy()->getId()) ||
					(playerAction != opponent->getAction())) {
				continue;
			}

			auto opponentPVal = static_cast<Affect>(
					opponent->getProperty(PROP_NAMES[0]).getValueAs<int>());
			if (Affect::ENEMY == opponentPVal) {
				hasEnemy = true;
			} else if (Affect::MEMBRANE == opponentPVal) {
				hasMembrane = true;
			}
		}
	}

	auto group = Affect::BLANK;
	if (hasEnemy) {
		group = Affect::ENEMY;
	} else if (hasMembrane) {
		group = Affect::MEMBRANE;
	}

	int iGroup = static_cast<int>(group);
	player->getProperty(PROP_NAMES[1]).setValue(iGroup);
}

/**
 * next を current に更新する
 *
 * その際状態が変われば、true, 変わらなければ false とする
 * @param player プレイヤ
 * @return 状態が変わったかどうか
 * @retval true 状態が変わった
 * @retval false 状態が変わらない
 */
bool AffectedPlayerRule::changesStatus(const std::shared_ptr<Player> player) {

	if (player->getProperty(PROP_NAMES[0]).getValueAs<int>() ==
			player->getProperty(PROP_NAMES[1]).getValueAs<int>()) {

		return false;
	}
	// 未来を現在へコピーする
	// group id
	player->getProperty(PROP_NAMES[0]).setValue(
			player->getProperty(PROP_NAMES[1]).getValueAs<int>());


	return true;
}

/*
 * プロパティの確認と初期化
 *
 * プロパティが無い、若しくはあっても数え上げが設定されていない場合(先頭プレイヤに限る)
 * プロパティを初期化する
 * @param player プレイヤ
 */
void AffectedPlayerRule::initProp(const std::shared_ptr<Player>& player) {

	for (int i = 0, max = PROP_NAMES.size(); i < max; ++i) {
		try {

			auto propName = PROP_NAMES.at(i);

			auto& prop = player->getProperty(propName);

			// 先頭プレイヤであり、"_"で始まらないプロパティ、かつ数え上げが無い場合は設定
			if ((player->getId() == 0) && (propName.front() != '_') &&
					(prop.getCountingMethod() == nullptr)) {
				std::shared_ptr<spd::core::PropertyCounting> counting =
						std::make_shared<spd::rule::counting::PropCount>();

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
				counting = std::make_shared<spd::rule::counting::PropCount>();
			}

			spd::core::Property p (PROP_NAMES.at(i), INIT_VALS.at(i), propType, counting);
			player->addProperty(p);
		}
	}
}

/*
 * 現在の状態をみて膜になるのかどうかのフィルタを作る
 * @param[in] allPlayers 全てのプレイヤ
 * @param[in] param パラメタ
 */
std::vector<bool> AffectedPlayerRule::filtering(
		const AllPlayer& allPlayers, const spd::param::Parameter& param) {

	auto strategies = param.getStrategyList();
	// それが膜になり得るかどうかのフィルタ
	std::vector<bool> filter(strategies.size() * 2, false);

	// どれぐらいの strategy x action が存在するのか
	int potential = 0;
	//存在するかどうかの可能性

	for (int i = 0, size = strategies.size(); i < size; ++i) {
		std::string shortStrategy = (strategies[i].first)->getShortStrategy();

		// C が含まれている
		if (shortStrategy.find("C") != std::string::npos) {
			potential++;
		}

		// D が含まれている
		if (shortStrategy.find("D") != std::string::npos) {
			potential++;
		}
	}

	// 現実にどれぐらいあるのか
	std::vector<bool> existence(strategies.size() * 2, false);
	for (auto& p : allPlayers) {
		// C = 0; D = 1
		int actionInt = spd::core::converter::actionToChar(p->getAction()) - 'C';
		if (!(existence[2 * (p->getStrategy()->getId()) + actionInt])) {
			existence[2 * (p->getStrategy()->getId()) + actionInt] = true;
			potential--;
		}
		// 全パターンでたら抜ける
		if (potential == 0) {
			break;
		}
	}

	int probabilityC = 0;
	int probabilityD = 0;

	for (int i = 0, size = strategies.size(); i < size; ++i) {
		// C, D 両方あるか
		if (existence[2 * i] && existence[2 * i + 1]) {
			filter[2 * i] = true;
			filter[2 * i + 1] = true;
		}
		// どのぐらい、それぞれがそんざいするか
		if (existence[2 * i]) {
			probabilityC++;
		}
		if (existence[2 * i + 1]) {
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



} /* namespace rule */
} /* namespace spd */
