/**
 * SpdRule.cpp
 *
 * @date 2012/09/08
 * @author katsumata
 */

#include "SpdRule.hpp"

namespace spd {
namespace rule {

/*
 * コンストラクタ
 */
SpdRule::SpdRule(std::string name) : name(name) {

	this->rulesBeforeOutput = std::vector<std::shared_ptr<Rule>>();
	this->rulesAfterOutput = std::vector<std::shared_ptr<Rule>>();
}

/*
 * ルール特有の初期化を実行
 * @param[in] allPlayers 全てのプレイヤ
 * @param[in] param パラメタ
 */
void SpdRule::init(
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param) {

	for (auto rule : this->rulesBeforeOutput) {
		for (auto& player : allPlayers) {
			rule->initialize(player, allPlayers, param);
		}
	}
	for (auto rule : this->rulesAfterOutput) {
		for (auto& player : allPlayers) {
			rule->initialize(player, allPlayers, param);
		}
	}
}

std::string SpdRule::toString() const {
	std::string result = "[ ";

	for (auto rule : this->rulesBeforeOutput) {
		result = result + "(" + rule->toString() + ")";
	}
	result += " | ";
	for (auto rule : this->rulesAfterOutput) {
		result = result + "(" + rule->toString() + ")";
	}
	result += " ]";

	return result;
}

} /* namespace core */
} /* namespace spd */

