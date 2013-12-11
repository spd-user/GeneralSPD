/**
 * NumberOutput.hpp
 *
 * @date 2013/01/16
 * @author katsumata
 */

#ifndef NUMBEROUTPUT_H_
#define NUMBEROUTPUT_H_

#include <fstream>
#include <string>
#include <memory>
#include "Output.hpp"

namespace spd {
namespace output {

/**
 * 数の出力を行うクラス
 */
class NumberOutput : public Output {
public:

	NumberOutput() : outputFile(new std::ofstream) {};

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
		return "number";
	}

private:

	/**
	 * 接頭辞
	 */
	const std::string PREFIX = "/number/spd_num_";

	/**
	 * 接尾辞
	 */
	const std::string SUFFIX = ".txt";

	// 出力ファイル
	std::unique_ptr<std::ofstream> outputFile;
};

} /* namespace output */
} /* namespace spd */
#endif /* NUMBEROUTPUT_H_ */
