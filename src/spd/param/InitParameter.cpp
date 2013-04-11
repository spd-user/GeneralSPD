/**
 * InitParameter.cpp
 *
 * @date 2012/09/20
 * @author katsumata
 */

#include <stdexcept>
#include "InitParameter.hpp"
#include "GenerateSpdRule.hpp"
#include "../core/Converter.hpp"
#include "../rule/SpdRule.hpp"

namespace spd {
namespace param {

/*
 * デフォルトコンストラクタ
 */
InitParameter::InitParameter() :
		playerNum(DEFAULT_PLAYER_NUM),
		endStep(DEFAULT_END_STEP),
		simCount(DEFAULT_SIM),
		startsFromCluster(DEFAULT_STARTS_CLUSTER),
		startClusterSize(DEFAULT_START_CLUSTER_SIZE),
		startsFixedAction(DEFAULT_STARTS_FIXED_ACTION),
		fixedAction(DEFAULT_FIXED_ACTION),
		memory(DEFAULT_MEMORY) {

	GenerateSpdRule g;
	std::string defaultRule = "SimpleBestRule";
	auto rule = g.generate(defaultRule);
	if (rule == nullptr) {
		throw std::runtime_error("Could not generate a rule and initialize a initParameter.");
	}

	properties = std::make_shared<std::vector<std::pair<std::string, std::string>>>();

	this->spdRule = rule;
}


/*
 * 表示
 */
void InitParameter::showParameter(std::ostream& out) const {

	// ユニット数
	out << "player-unit = " << playerNum << "\n";
	// 終了ステップ数
	out << "end-steps = " << endStep << "\n";

	// シミュレーション回数
	out << "sim = " << simCount << "\n";

	// 更新ルール
	out << "rule = " << spdRule->getName() << "\n";
	out << "# " << spdRule->toString() << "\n";

	if (startsFixedAction) {
		out << "fixed-action = " << spd::core::converter::actionToChar(fixedAction) << "\n";
	} else {
		out << "# Not fixed action\n";
	}

	// 初期状態は中央セル(クラスタ)のみ
	if (startsFromCluster) {
		out << "lonely-cluster = " << startClusterSize << "\n";
	} else {
		out << "# Start from a random situation\n";
	}

	// プロパティ
	for (int i = 0, propertySize = properties->size(); i < propertySize; ++i) {
		out << "property = " << properties->at(i).first << ":" << properties->at(i).second << "\n";
	}

	// 使用できるメモリ(バイト数)
	out << "memory = " << memory << "\n";
}


} /* namespace param */
} /* namespace spd */
