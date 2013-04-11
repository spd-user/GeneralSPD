/**
 * RandomParameter.hpp
 *
 * @date 2012/11/09
 * @author katsumata
 */

#ifndef RANDOMPARAMETER_H_
#define RANDOMPARAMETER_H_

#include <random>
#include "IShowParameter.hpp"

namespace spd {
namespace param {

/**
 * 乱数に関わるパラメタを表すクラス
 */
class RandomParameter : public IShowParameter {

public:

	/**
	 * デフォルトコンストラクタ
	 */
	RandomParameter();

	/**
	 * 生成エンジンを取得
	 * @return 生成エンジン
	 */
	std::mt19937_64& getEngine() {
		return engine;
	}

	/**
	 * 初期で切り捨てる乱数の数を取得
	 * @return 生成した乱数の数
	 */
	unsigned long long getDiscardNum() const {
		return discardNum;
	}

	/**
	 * 初期で切り捨てる乱数の数を設定
	 * @param[in] discard 切り捨てる乱数の数
	 */
	void setDiscardNum(unsigned long long discard) {
		this->discardNum = discard;
	}

	/**
	 * シミュレーション中で作成した乱数の数を取得する
	 * @return シミュレーション中で作成した乱数の数
	 */
	unsigned long long getGenerated() const {
		return generatedNum;
	}

	/**
	 * シミュレーション中で作成した乱数の数を加える
	 * @param[in] generated 新たに生成した乱数の数
	 */
	void addGenerated(unsigned long long generated) {
		this->generatedNum += generated;
	}

	/**
	 * 乱数の種を取得
	 * @return 乱数の種
	 */
	unsigned int getSeed() const {
		return seed;
	}

	/**
	 * 乱数の種を設定
	 * @param seed 乱数の種
	 */
	void setSeed(unsigned int seed) {
		engine.seed(seed);
		this->seed = seed;
	}

	/**
	 * パラメタを出力する
	 * @param[in] out 出力先
	 */
	void showParameter(std::ostream& out) const;

private:

	/**
	 * 乱数の種
	 */
	unsigned int seed;

	// 初めに切り捨てる乱数の数
	unsigned long long discardNum;

	// シミュレーション中に生成された数
	unsigned long long generatedNum;

	/**
	 * 生成エンジン
	 */
	std::mt19937_64 engine;
};

} /* namespace param */
} /* namespace spd */
#endif /* RANDOMPARAMETER_H_ */
