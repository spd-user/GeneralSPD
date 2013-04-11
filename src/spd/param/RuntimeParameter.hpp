/**
 * RuntimeParameter.hpp
 *
 * @date 2012/09/20
 * @author katsumata
 */

#ifndef RUNTIMEPARAMETERETER_H_
#define RUNTIMEPARAMETERETER_H_

#include "IShowParameter.hpp"
#include "../core/Action.hpp"

namespace spd {
namespace param {

/**
 * 実行時に用いるパラメタを保持するクラス
 */
class RuntimeParameter : public IShowParameter {

public:

	/**
	 * デフォルト値で初期化を行う
	 */
	RuntimeParameter();

	/**
	 * 自己対戦するかどうかを取得
	 * @return 自己対戦するかどうか
	 * @retval true 自己対戦を行う場合
	 * @retval false 自己対戦をしない場合
	 */
	bool isSelfInteraction() const {
		return selfInteraction;
	}

	/**
	 * 自己対戦するかどうかを設定する
	 * @param[in] selfInteraction 自己対戦するかどうか
	 */
	void setSelfInteraction(bool selfInteraction) {
		this->selfInteraction = selfInteraction;
	}

	/**
	 * 戦略更新周期を取得する
	 * @return 戦略更新周期
	 */
	int getStrategyUpdateCycle() const {
		return strategyUpdateCycle;
	}

	/**
	 * 戦略更新周期を設定
	 * @param[in] strategyUpdateCycle 戦略更新周期
	 */
	void setStrategyUpdateCycle(int strategyUpdateCycle) {
		this->strategyUpdateCycle = strategyUpdateCycle;
	}


	/**
	 * 利得行列全てを取得する
	 * @return 利得行列
	 */
	double (*getPayoffMatrix())[2] {
		return payoffMatrix;
	}

	/**
	 * 自身の利得行を取得する
	 * @param[in] own 自身の行動
	 * @return 自身の利得行
	 */
	double* getPayoffRow(Action own) {
		return this->payoffMatrix[static_cast<int>(own)];
	}

	/**
	 * 自身と相手の行動を設定して、利得を取得する
	 * @param[in] own 自身の行動
	 * @param[in] opponent 相手の行動
	 * @return 対戦による利得
	 */
	const double getPayoff(Action own, Action opponent) const {
		return this->payoffMatrix[static_cast<int>(own)][static_cast<int>(opponent)];
	}

	/**
	 * 両方の協調の報酬を取得する
	 * @return 両方の協調の報酬
	 */
	const double getPayoffR() const {
		return getPayoff(Action::ACTION_C, Action::ACTION_C);
	}

	/**
	 * 両方の協調の報酬を設定する
	 * @param[in] value 利得
	 */
	void setPayoffR(double value) {
		payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_C)] = value;
	}


	/**
	 * お人好しの利得を取得する
	 * @return お人好しの利得
	 */
	const double getPayoffS() const {
		return getPayoff(Action::ACTION_C, Action::ACTION_D);
	}

	/**
	 * お人好しの利得を設定する
	 * @param[in] value 利得
	 */
	void setPayoffS(double value) {
		payoffMatrix[static_cast<int>(Action::ACTION_C)][static_cast<int>(Action::ACTION_D)] = value;
	}

	/**
	 * 裏切りへの誘惑を取得する
	 * @return 裏切りへの誘惑
	 */
	const double getPayoffT() const {
		return getPayoff(Action::ACTION_D, Action::ACTION_C);
	}

	/**
	 * 裏切りへの誘惑を設定する
	 * @param[in] value 利得
	 */
	void setPayoffT(double value) {
		payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_C)] = value;
	}

	/**
	 * 両方が裏切った懲罰を取得する
	 * @return 両方が裏切った懲罰
	 */
	const double getPayoffP() const {
		return getPayoff(Action::ACTION_D, Action::ACTION_D);
	}

	/**
	 * 両方が裏切った懲罰を設定する
	 * @param[in] value 利得
	 */
	void setPayoffP(double value) {
		payoffMatrix[static_cast<int>(Action::ACTION_D)][static_cast<int>(Action::ACTION_D)] = value;
	}

	/**
	 * パラメタを出力する
	 * @param[in] out 出力先
	 */
	void showParameter(std::ostream& out) const;

	/**
	 * パラメタの保存を行う
	 */
	void store();

	/**
	 * パラメタを保存した値からリストアする
	 */
	void restore();

private:
	// デフォルト値

	static const int DEFAULT_STRATEGY_UPDATE_CYCLE = 1;

	static const bool DEFAULT_SELF_INTERACTION = false;

	static constexpr double DEFAULT_P_VALUE = 0.0;
	static constexpr double DEFAULT_R_VALUE = 1.0;
	static constexpr double DEFAULT_S_VALUE = 0.0;
	static constexpr double DEFAULT_T_VALUE = 1.80001;

	// パラメタの実態
	// 戦略更新周期
	int strategyUpdateCycle;

	// 自己対戦を行う
	bool selfInteraction;

	// 利得行列
	double payoffMatrix[2][2];

	// パラメタのストア値
	// 戦略更新周期
	int s_strategyUpdateCycle;
	// 自己対戦
	bool s_selfInteraction;
	// 利得行列
	double s_payoffMatrix[2][2];

};

} /* namespace param */
} /* namespace spd */
#endif /* RUNTIMEPARAMETERETER_H_ */
