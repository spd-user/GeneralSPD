/**
 * AllRules.hpp
 *
 * @date 2013/01/09
 * @author katsumata
 */

#ifndef ALLRULES_H_
#define ALLRULES_H_

// 状態のストアルール
#include "PromoteStateRule.hpp"

// 行動ルール
#include "action/SimpleActionRule.hpp"

// ゲームルール
#include "game/SimpleSumGameRule.hpp"

// 戦略ルール
#include "strategy/BestStrategyRule.hpp"


// TODO プロパティのテスト
#include "property/PropertyTest.hpp"

#endif /* ALLRULES_H_ */
