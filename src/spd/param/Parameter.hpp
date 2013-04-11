/**
 * Parameter.hpp
 *
 * @date 2012/09/05
 * @author katsumata
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <string>
#include <vector>
#include <utility>
#include <memory>

#include "IShowParameter.hpp"

namespace spd {
namespace core {
class Strategy;
class PlayerMaker;
}

namespace param {
class InitParameter;
class OutputParameter;
class RuntimeParameter;
class NeighborhoodParameter;
class RandomParameter;


/**
 * パラメタを表すクラス
 */
class Parameter : public IShowParameter {
public:

	/**
	 * デフォルト値で初期化を行う
	 */
	Parameter();

	/**
	 * 戦略と初期状態でのその戦略の割合のリストを取得
	 * @return 戦略と初期状態でのその戦略の割合のリスト
	 */
	const std::vector<std::pair<std::shared_ptr<core::Strategy>, int>>& getStrategyList() const;

	/**
	 * 戦略リストに戦略と初期状態での割合を追加
	 *
	 * 同じ戦略がある場合、割合を追加する
	 * @param[in] strategyPair 戦略と初期状態での割合
	 */
	void addStrategy(std::pair<std::shared_ptr<core::Strategy>, int> strategyPair);

	/**
	 * 戦略リストを空にする
	 */
	void clearList() {
		strategyList.clear();
	}


	/**
	 * 初期化用パラメタの取得
	 * @return 初期化用パラメタ
	 */
	const std::shared_ptr<InitParameter>& getInitialParameter() const {
		return initParam;
	}

	/**
	 * 近傍用パラメタの取得
	 * @return 近傍用パラメタ
	 */
	const std::shared_ptr<NeighborhoodParameter>& getNeighborhoodParameter() const {
		return neighborParam;
	}

	/**
	 * 出力用パラメタの取得
	 * @return 出力用パラメタ
	 */
	const std::shared_ptr<OutputParameter>& getOutputParameter() const {
		return outputParam;
	}

	/**
	 * 実行時用パラメタの取得
	 * @return 実行時用パラメタ
	 */
	const std::shared_ptr<RuntimeParameter>& getRuntimeParameter() const {
		return runtimeParam;
	}

	/**
	 * 乱数用パラメタの取得
	 * @return 乱数用パラメタ
	 */
	const std::shared_ptr<RandomParameter>& getRandomParameter() const {
		return randomParam;
	}

	/**
	 * プレイヤ作成方法の設定
	 * @param[in] playerMaker プレイヤメーカー
	 */
	void setPlayerMaker(const std::shared_ptr<spd::core::PlayerMaker>& playerMaker) {
		pm = playerMaker;
	}

	/**
	 * プレイヤ作成方法の取得
	 * @return プレイヤメーカー
	 */
	const std::shared_ptr<spd::core::PlayerMaker>& getPlayerMaker() const {
		return pm;
	}

	/**
	 * シミュレーションで使用するコア数を取得
	 * @return シミュレーションで使用するコア数
	 */
	int getCore() const {
		return core;
	}

	/**
	 * シミュレーションで使用するコア数
	 * @param[in] シミュレーションで使用するコア数
	 */
	void setCore(int core) {
		this->core = core;
	}

	/**
	 * すべてのパラメタを出力する
	 * @param[in] out 出力先
	 */
	void showParameter(std::ostream& out) const;

	/**
	 * パラメタの保存を行う。
	 *
	 * @par
	 * 保存される値は runtimeParameter の値と戦略に限定される
	 */
	void store();

	/**
	 * パラメタを保存した値からリストアする
	 */
	void restore();

private:
	// 戦略(戦略，初期割合)のリスト
	std::vector<std::pair<std::shared_ptr<core::Strategy>, int>> strategyList;
	// 保存用
	std::vector<std::pair<std::shared_ptr<core::Strategy>, int>> s_strategyList;

	// 初期化パラメタ
	std::shared_ptr<InitParameter> initParam;

	// 近傍用パラメタ
	std::shared_ptr<NeighborhoodParameter> neighborParam;

	// 出力用パラメタ
	std::shared_ptr<OutputParameter> outputParam;

	// 実行時パラメタ
	std::shared_ptr<RuntimeParameter> runtimeParam;

	// 乱数パラメタ
	std::shared_ptr<RandomParameter> randomParam;

	// プレイヤ作成クラス
	std::shared_ptr<spd::core::PlayerMaker> pm;

	// コア数
	int core;
};

} /* namespace core */
} /* namespace spd */
#endif /* PARAMETER_H_ */
