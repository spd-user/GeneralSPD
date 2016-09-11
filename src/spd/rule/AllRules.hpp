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
#include "game/AverageGameRule.hpp"
#include "game/UniformDiscountDistanceGameRule.hpp"
#include "game/InverseSquareDiscountDistanceGameRule.hpp"

// 戦略ルール
#include "strategy/BestStrategyRule.hpp"

// プロパティ
#include "property/MembraneDetectRule.hpp"
#include "property/AffectedPlayerRule.hpp"

// TODO テストルール
#include "property/PropertyTest.hpp"

#endif /* ALLRULES_H_ */
