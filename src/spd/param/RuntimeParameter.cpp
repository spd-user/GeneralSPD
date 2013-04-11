/**
 * RuntimeParameter.cpp
 *
 * @date 2012/09/20
 * @author katsumata
 */

#include "RuntimeParameter.hpp"

namespace spd {
namespace param {

/*
 * デフォルトコンストラクタ
 */
RuntimeParameter::RuntimeParameter() :
		strategyUpdateCycle(DEFAULT_STRATEGY_UPDATE_CYCLE),
		selfInteraction(DEFAULT_SELF_INTERACTION) ,
		s_strategyUpdateCycle(DEFAULT_STRATEGY_UPDATE_CYCLE),
		s_selfInteraction(DEFAULT_SELF_INTERACTION) {

	payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_C)] = DEFAULT_R_VALUE;
	payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_D)] = DEFAULT_S_VALUE;
	payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_C)] = DEFAULT_T_VALUE;
	payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_D)] = DEFAULT_P_VALUE;

	s_payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_C)] = DEFAULT_R_VALUE;
	s_payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_D)] = DEFAULT_S_VALUE;
	s_payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_C)] = DEFAULT_T_VALUE;
	s_payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_D)] = DEFAULT_P_VALUE;

}

/*
 * パラメタの保存を行う
 */
void RuntimeParameter::store() {
	// 戦略更新周期
	s_strategyUpdateCycle = strategyUpdateCycle;
	// 自己対戦
	s_selfInteraction = selfInteraction;
	// 利得行列
	s_payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_C)] =
			payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_C)];
	s_payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_D)] =
			payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_D)];
	s_payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_C)] =
			payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_C)];
	s_payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_D)] =
			payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_D)];

}


/*
 * パラメタを保存した値からリストアする
 */
void RuntimeParameter::restore() {
	// 戦略更新周期
	strategyUpdateCycle = s_strategyUpdateCycle;
	// 自己対戦
	selfInteraction = s_selfInteraction;
	// 利得行列
	payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_C)] =
			s_payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_C)];
	payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_D)] =
			s_payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_D)];
	payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_C)] =
			s_payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_C)];
	payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_D)] =
			s_payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_D)];

}

/*
 * パラメタの表示
 */
void RuntimeParameter::showParameter(std::ostream& out) const {

	out << "strategy-update-cycle = " << strategyUpdateCycle << "\n";

	if (selfInteraction) {
		out << "self-interaction = true\n";
	} else {
		out << "# Do Not self-interaction\n";
	}

	out << "payoff-R = " <<
			payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_C)] << "\n";

	out << "payoff-S = " <<
			payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_D)] << "\n";

	out << "payoff-T = " <<
			payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_C)] << "\n";

	out << "payoff-P = " <<
			payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_D)] << "\n";

}

} /* namespace param */
} /* namespace spd */
