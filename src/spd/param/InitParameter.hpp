/**
 * InitParameter.hpp
 *
 * @date 2012/09/20
 * @author katsumata
 */

#ifndef INITPARAMETERETER_H_
#define INITPARAMETERETER_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "IShowParameter.hpp"
#include "../core/Action.hpp"

namespace spd {
namespace rule {
class SpdRule;
}
namespace param {

/**
 * シミュレーションの初期化以降に書き換わる可能性のないパラメタを保持するクラス
 */
class InitParameter : public IShowParameter {

public:

	/**
	 * デフォルト値で初期化を行う
	 */
	InitParameter();

	/**
	 * 初期状態で固定するプレイヤの行動を取得
	 * @return 初期状態に固定される行動
	 */
	Action getFixedAction() const {
		return this->fixedAction;
	}

	/**
	 * 初期状態で固定するプレイヤの行動を設定
	 * @param[in] fixedAction 初期状態に固定される行動
	 */
	void setFixedAction(Action fixedAction) {
		this->fixedAction = fixedAction;
	}

	/**
	 * 使用可能メモリ容量を取得
	 * @return 使用可能メモリ容量(バイト)
	 */
	long int getMemory() const {
		return memory;
	}

	/**
	 * 使用可能メモリ容量(バイト)を設定
	 * @param[in] memory 使用できるメモリ容量
	 */
	void setMemory(long int memory) {
		this->memory = memory;
	}


	/**
	 * 固定行動の初期状態から開始するかを取得
	 * @return 固定行動の初期状態から開始するか
	 * @retval true 固定行動の初期状態から開始する場合
	 * @retval false 固定行動の初期状態から開始しない場合
	 */
	bool startsInitialFixedAction() const {
		return startsFixedAction;
	}

	/**
	 * 固定行動の初期状態から開始するかを設定
	 * @param[in] setsFixedAction 固定行動の初期状態から開始するか
	 */
	void setStartsInitialFixedAction(bool setsFixedAction) {
		this->startsFixedAction = setsFixedAction;
	}

	/**
	 * 初期状態で中央だけに配置する、最初の戦略のクラスタサイズを取得
	 * @note クラスタ形状は、トポロジーの形状に従う
	 * @return 初期状態で中央だけに配置する、最初の戦略のクラスタサイズ
	 */
	int getStartClusterSize() const {
		return startClusterSize;
	}

	/**
	 * 初期状態で中央だけに配置する、最初の戦略のクラスタサイズを設定
	 * @param[in] startClusterSize 初期状態のクラスタサイズ
	 */
	void setStartClusterSize(int startClusterSize) {
		this->startClusterSize = startClusterSize;
	}

	/**
	 * 初期状態でクラスタを配置するかを取得
	 * @return 初期状態でクラスタを配置するか
	 * @retval true 初期状態でクラスタを配置する場合
	 * @retval false 初期状態でクラスタを配置しない場合
	 */
	bool startsCluster() const {
		return startsFromCluster;
	}

	/**
	 * 初期状態でクラスタを配置するかを設定
	 * @param[in] startsCluster 初期状態でクラスタを配置するか
	 */
	void setStartsCluster(bool startsCluster) {
		this->startsFromCluster = startsCluster;
	}

	/**
	 * ユニット数を取得
	 * @return ユニット数
	 */
	int getPlayerNum() const {
		return playerNum;
	}

	/**
	 * ユニット数を設定
	 * @param[in] playerNum ユニット数
	 */
	void setPlayerNum(int playerNum) {
		this->playerNum = playerNum;
	}


	/**
	 * ステップ数を取得
	 * @return ステップ数
	 */
	int getEndStep() const {
		return endStep;
	}

	/**
	 * ステップ数を設定
	 * @param[in] endStep 終了ステップ数
	 */
	void setEndStep(int endStep) {
		this->endStep = endStep;
	}

	/**
	 * シミュレーション回数を取得
	 * @return シミュレーション回数
	 */
	int getSimCount() const {
		return simCount;
	}

	/**
	 * シミュレーション回数を設定
	 * @param[in] simCount シミュレーション回数
	 */
	void setSimCount(int simCount) {
		this->simCount = simCount;
	}

	/**
	 * SPDルールを取得
	 * @return SPDルール
	 */
	const std::shared_ptr<spd::rule::SpdRule>& getSpdRule() const {
		return this->spdRule;
	}

	/**
	 * SPDルールを設定
	 * @param[in] spdRule SPDルール
	 */
	void setSpdRule(const std::shared_ptr<spd::rule::SpdRule>& spdRule) {
		this->spdRule = spdRule;
	}

	/**
	 * プロパティの種類と初期値
	 * @return プロパティの種類と初期値
	 */
	const std::shared_ptr<std::vector<std::pair<std::string, std::string>>> getProperties() const {
		return this->properties;
	}

	/**
	 * パラメタを出力する
	 * @param out 出力先
	 */
	void showParameter(std::ostream& out) const;

private:
	// デフォルト値
	static const int DEFAULT_PLAYER_NUM = 100;
	static const int DEFAULT_END_STEP = 20;

	static const int DEFAULT_SIM = 1;

	static const bool DEFAULT_STARTS_CLUSTER = false;
	static const int DEFAULT_START_CLUSTER_SIZE = -1;

	static const bool DEFAULT_STARTS_FIXED_ACTION = false;
	static const Action DEFAULT_FIXED_ACTION = Action::ACTION_C;

	static const long int DEFAULT_MEMORY = 0;

	// パラメタの宣言
	// ユニット数
	int playerNum;
	// 終了ステップ数
	int endStep;

	// シミュレーション回数
	int simCount;

	// 更新ルール
	std::shared_ptr<spd::rule::SpdRule> spdRule;

	// 初期状態は中央セル(クラスタ)のみ
	bool startsFromCluster;
	// 初期状態の中央セル(クラスタ)サイズ
	int startClusterSize;
	// 一定の行動で始めるかどうか
	bool startsFixedAction;
	// 初期状態でのプレイヤの行動
	Action fixedAction;

	// プロパティ名と初期値
	std::shared_ptr<std::vector<std::pair<std::string, std::string>>> properties;

	// 使用できるメモリ(バイト数)
	long int memory;
};

} /* namespace param */
} /* namespace spd */
#endif /* INITPARAMETERETER_H_ */
