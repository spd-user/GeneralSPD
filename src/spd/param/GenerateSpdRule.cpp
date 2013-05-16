/**
 * GenerateSpdRule.cpp
 *
 * @date 2013/01/09
 * @author katsumata
 */

#include <stdexcept>
#include <algorithm>
#include "GenerateSpdRule.hpp"
#include "../rule/SpdRule.hpp"
#include "../rule/AllRules.hpp"

namespace spd {
namespace param {

using namespace std;

GenerateSpdRule::GenerateSpdRule() {

	// 作成可能ルールを設定
	string bestRuleName = "SimpleBestRule";
	auto bestRule = make_shared<spd::rule::SpdRule>(bestRuleName);
	bestRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleActionRule>());

	bestRule->addRuleAfterOutput(make_shared<spd::rule::SimpleSumGameRule>());
	bestRule->addRuleAfterOutput(make_shared<spd::rule::PromoteStateRule>());
	bestRule->addRuleAfterOutput(make_shared<spd::rule::BestStrategyRule>());

	// TODO
	// testProp ルールを加える
	//bestRule->addRuleBeforeOutput(make_shared<spd::rule::PropertyTest>());

	transform(bestRuleName.begin(), bestRuleName.end(), bestRuleName.begin(), ::tolower);

	// ルールの追加
	map<string, shared_ptr<spd::rule::SpdRule>> m {
		{bestRuleName, bestRule}
	};

	this->spdRuleMap = m;
}

/*
 * 文字列から、ルールを作成する
 * @param[in] spdRule ルール名
 * @throw invalid_argument 文字列が空の場合または、ルール名に対応する構造を作成できない場合
 */
std::shared_ptr<spd::rule::SpdRule> GenerateSpdRule::generate(const std::string& spdRule) const {

	if (spdRule.empty()) {
		throw std::invalid_argument("SpdRule strings could not to be empty string.");
	}

	std::string spdRuleName = spdRule;
	transform(spdRuleName.begin(), spdRuleName.end(), spdRuleName.begin(), ::tolower);

	auto itr = this->spdRuleMap.find(spdRuleName);
	if (itr != this->spdRuleMap.end()) {
		return itr->second;
	}

	std::string err = "";
	for (auto m : this->spdRuleMap) {
		err += " " + m.first;
	}
	throw std::invalid_argument("Could not generate rule for " + spdRule + ".\n"
			"Settable rule(s): [" + err + " ]");
}

/*
 * SPDルール名とSPDルールの対応を取得
 */
const std::map<std::string, std::shared_ptr<spd::rule::SpdRule>>& GenerateSpdRule::getSpdRuleMap() const {
	return this->spdRuleMap;
}

} /* namespace param */
} /* namespace spd */
