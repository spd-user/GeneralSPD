/**
 * GenerateOutput.hpp
 *
 * @date 2012/12/31
 * @author katsumata
 */

#ifndef GENERATEOUTPUT_H_
#define GENERATEOUTPUT_H_

#include <memory>
#include <map>
#include <string>
#include <tuple>
#include <vector>


namespace spd {

namespace output {
class Output;
}
namespace param {

/**
 * 出力方法の生成を行うクラス
 */
class GenerateOutput {
public:

	/**
	 * コンストラクタ
	 */
	GenerateOutput();

	/**
	 * 出力方法と開始ステップ、終了ステップと間隔の文字列から、出力の組を作成する
	 *
	 * 指定がない場合は、開始ステップは0、終了ステップは-1、間隔は1で設定される。
	 * マイナスの場合は最終ステップを除き、絶対値をとる。
	 * @param[in] output 出力方法と各設定ステップを「:」で区切った文字列
	 * @return 出力ペア
	 * @throw invalid_argument 出力名に対応する方法を作成できない場合
	 */
	std::tuple<std::shared_ptr<output::Output>, int, int, int> generate(const std::string& output) const;

	/**
	 * 出力方法と実態の対応を取得
	 * @return 出力方法と実態の対応
	 */
	const std::map<std::string, std::shared_ptr<spd::output::Output>>& getOutputMap() const;


private:

	/**
	 * 出力処理名と出力を結ぶmap
	 */
	std::map<std::string, std::shared_ptr<spd::output::Output>> outputMap;

	/**
	 * 文字列を、出力方法と初期ステップ、終了ステップ、間隔に分割する
	 * @param[in] output 最低でも出力方法がある文字列
	 * @return 分割されたタプル
	 */
	std::vector<std::string> separateOutputString(std::string output) const;

};

} /* namespace param */
} /* namespace spd */
#endif /* GENERATEOUTPUT_H_ */
