/**
 * PropertyOutput.hpp
 *
 * @date 2013/04/18
 * @author katsumata
 */

#ifndef PROPERTYOUTPUT_HPP_
#define PROPERTYOUTPUT_HPP_

#include <vector>
#include <memory>
#include <fstream>

#include "Output.hpp"

namespace spd {
namespace output {

/**
 * プレイヤのプロパティ出力を行うクラス
 */
class PropertyOutput: public spd::output::Output {

	/**
	 * 空間の情報を出力する
	 * @param[in, out] space 空間
	 * @return 圧縮する必要はないので second が false で、first は空
	 */
	virtual std::pair<std::string, bool> output(spd::core::Space& space);

	/**
	 * 初期化を行う
	 * @param[in, out] space 空間
	 * @param[in, out] param パラメタ
	 * @throw std::runtime_error ファイルを開けない場合
	 */
	virtual void init(spd::core::Space& space, spd::param::Parameter& param);
	
	/**
	 * クラス情報の文字出力
	 */
	std::string toString() const {
		return "property";
	}
	
private:

	// 共通接頭辞
	const std::string PREFIX = "/property/spd_";

	// 共通接尾辞
	const std::string SUFFIX = ".txt";

	// 出力ファイル群
	std::vector<std::shared_ptr<std::ofstream>> outputFiles;
};

} /* namespace output */
} /* namespace spd */
#endif /* PROPERTYOUTPUT_HPP_ */
