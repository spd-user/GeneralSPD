/**
 * BinaryOutput.hpp
 *
 * @date 2013/02/08
 * @author katsumata
 */

#ifndef BINARYOUTPUT_HPP_
#define BINARYOUTPUT_HPP_

#include "Output.hpp"
#include <sstream>

namespace spd {
namespace output {


/**
* バイナリシリアライズを行うクラス
*/
class BinaryOutput : public Output {
public:

	/**
	 * 空間の情報を出力する
	 * @param[in, out] space 空間
	 * @return 圧縮するので second が trueで、first にファイル名のペア
	 */
	std::pair<std::string, bool> output(spd::core::Space& space);

	/**
	 * 初期化
	 *
	 * @par
	 * configファイルの作成
	 *
	 * @param[in, out] space 空間
	 * @param[in, out] param パラメタ
	 */
	void init(spd::core::Space& space, spd::param::Parameter& param);

	/**
	 * クラス情報の文字出力
	 */
	std::string toString() const {
		return "dump";
	}

private:

	/**
	 * 出力ディレクトリ名
	 */
	const std::string DIR = "/dump";

	/**
	 * ファイルの接頭辞
	 */
	const std::string PREFIX = "/spd_dump_";

	/**
	 * ファイルの接尾辞
	 */
	const std::string SUFFIX = ".mpac";

	// マニピュレータの設定
	void setZeroPadding(std::ostringstream& oss, int width, int val);
};


} /* namespace output */
} /* namespace spd */
#endif /* BINARYOUTPUT_HPP_ */
