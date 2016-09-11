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

	// ルールを設定
	// シンプルな総和・最高利得ゲーム
	string bestRuleName = "simple_best_rule";
	auto bestRule = make_shared<spd::rule::SpdRule>(bestRuleName);
	bestRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleActionRule>());
	bestRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleSumGameRule>());
	bestRule->addRuleBeforeOutput(make_shared<spd::rule::PromoteStateRule>());

	bestRule->addRuleAfterOutput(make_shared<spd::rule::BestStrategyRule>());
	// TODO
	// testProp ルールを加える
	//bestRule->addRuleBeforeOutput(make_shared<spd::rule::PropertyTest>());

	transform(bestRuleName.begin(), bestRuleName.end(), bestRuleName.begin(), ::tolower);


	// 平均・最高利得ゲーム
	string aveRuleName = "average_rule";
	auto aveRule = make_shared<spd::rule::SpdRule>(aveRuleName);
	aveRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleActionRule>());
	aveRule->addRuleBeforeOutput(make_shared<spd::rule::AverageGameRule>());
	aveRule->addRuleBeforeOutput(make_shared<spd::rule::PromoteStateRule>());

	aveRule->addRuleAfterOutput(make_shared<spd::rule::BestStrategyRule>());

	transform(aveRuleName.begin(), aveRuleName.end(), aveRuleName.begin(), ::tolower);


	// 距離均等割引ゲーム
	string uniDiscoutRuleName = "uniform_discount_rule";
	auto uniDiscountRule = make_shared<spd::rule::SpdRule>(uniDiscoutRuleName);
	uniDiscountRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleActionRule>());
	uniDiscountRule->addRuleBeforeOutput(make_shared<spd::rule::UniformDiscountDistance>());
	uniDiscountRule->addRuleBeforeOutput(make_shared<spd::rule::PromoteStateRule>());

	uniDiscountRule->addRuleAfterOutput(make_shared<spd::rule::BestStrategyRule>());

	transform(uniDiscoutRuleName.begin(), uniDiscoutRuleName.end(), uniDiscoutRuleName.begin(), ::tolower);


	// 距離逆二乗割引ゲーム
	string inverseSquareDiscoutRuleName = "inverse_square_rule";
	auto inverseSquareDiscoutRule = make_shared<spd::rule::SpdRule>(inverseSquareDiscoutRuleName);
	inverseSquareDiscoutRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleActionRule>());
	inverseSquareDiscoutRule->addRuleBeforeOutput(make_shared<spd::rule::InverseSquareDiscountDistance>());
	inverseSquareDiscoutRule->addRuleBeforeOutput(make_shared<spd::rule::PromoteStateRule>());

	uniDiscountRule->addRuleAfterOutput(make_shared<spd::rule::BestStrategyRule>());

	transform(inverseSquareDiscoutRuleName.begin(), inverseSquareDiscoutRuleName.end(), inverseSquareDiscoutRuleName.begin(), ::tolower);

	// ルールを設定
	// 膜チェックだけを行う、シンプルルール
	string memRuleName = "membrane_check_simple_rule";
	auto memRule = make_shared<spd::rule::SpdRule>(bestRuleName);
	memRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleActionRule>());
	memRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleSumGameRule>());
	memRule->addRuleBeforeOutput(make_shared<spd::rule::PromoteStateRule>());
	memRule->addRuleBeforeOutput(make_shared<spd::rule::MembraneDetectRule>());

	memRule->addRuleAfterOutput(make_shared<spd::rule::BestStrategyRule>());


	transform(memRuleName.begin(), memRuleName.end(), memRuleName.begin(), ::tolower);


	// 膜チェックと、影響を調べる、シンプルルール
	string fullRuleName = "membrane_and_effect_check_simple_rule";
	auto fullRule = make_shared<spd::rule::SpdRule>(bestRuleName);
	fullRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleActionRule>());
	fullRule->addRuleBeforeOutput(make_shared<spd::rule::SimpleSumGameRule>());
	fullRule->addRuleBeforeOutput(make_shared<spd::rule::PromoteStateRule>());
	fullRule->addRuleBeforeOutput(make_shared<spd::rule::MembraneDetectRule>());
	fullRule->addRuleBeforeOutput(make_shared<spd::rule::AffectedPlayerRule>());

	fullRule->addRuleAfterOutput(make_shared<spd::rule::BestStrategyRule>());


	transform(fullRuleName.begin(), fullRuleName.end(), fullRuleName.begin(), ::tolower);


	// ルールの追加
	map<string, shared_ptr<spd::rule::SpdRule>> m {
		{bestRuleName, bestRule},
		{aveRuleName, aveRule},
		{uniDiscoutRuleName, uniDiscountRule},
		{inverseSquareDiscoutRuleName, inverseSquareDiscoutRule},
		{memRuleName, memRule},
		{fullRuleName, fullRule}
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
