/**
 * PayoffOutput.hpp
 *
 * @date 2013/01/16
 * @author katsumata
 */

#ifndef PAYOFFOUTPUT_H_
#define PAYOFFOUTPUT_H_

#include <fstream>
#include <string>
#include "Output.hpp"

namespace spd {
namespace output {

/**
 * 利得の出力を行うクラス
 */
class PayoffOutput : public Output {
public:

	/**
	 * 空間の情報を出力する
	 * @param[in, out] space 空間
	 * @return 圧縮しないので second がfalse で、firstは空
	 * @throw std::runtime_error プレイヤの戦略が戦略リストにない場合
	 */
	std::pair<std::string, bool> output(spd::core::Space& space);

	/**
	 * 初期化
	 * @param[in, out] space 空間
	 * @param[in, out] param パラメタ
	 * @throw std::runtime_error ファイルを開けない場合
	 */
	void init(spd::core::Space& space, spd::param::Parameter& param);

	/**
	 * クラス情報の文字出力
	 */
	std::string toString() const {
		return "payoff";
	}

private:

	// 接頭辞
	const std::string PREFIX = "/payoff/spd_payoff_";

	// 接尾辞
	const std::string SUFFIX = ".txt";

	// 出力ファイル
	std::ofstream outputFile;
};

} /* namespace output */
} /* namespace spd */
#endif /* PAYOFFOUTPUT_H_ */
