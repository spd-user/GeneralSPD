/**
 * ParseParam.hpp
 *
 * @date 2012/12/29
 * @author katsumata
 */

#ifndef PARSEPARAM_H_
#define PARSEPARAM_H_

#include <utility>
#include <string>
#include <memory>

namespace spd {
namespace core {
class Strategy;
}
namespace param {
class Parameter;
class NeighborhoodParameter;
namespace parser {

/**
 * パラメタの解析を行うクラス
 */
class ParseParam {
public:

	/**
	 * コマンドライン引数からパラメタの解析を行う
	 * @param[in] argc コマンドライン引数
	 * @param[in] argv コマンドライン引数値
	 * @param[in, out] param パラメタ保持クラス
	 */
	void parse(int argc, char* argv[], Parameter& param) const;

private:

	/**
	 * コマンドラインの戦略引数から戦略と割合のペアを作る
	 * @param[in] strategyOptValue コマンドラインの戦略引数の値
	 * @param[in] neiParam 近傍関係パラメタ
	 * @return 戦略と割合のペア
	 */
	std::pair<std::shared_ptr<spd::core::Strategy>, int>
		parseStrategyOptionValue(
				const std::string& strategyOptValue,
				const std::shared_ptr<NeighborhoodParameter>& neiParam) const;

};

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
#endif /* PARSEPARAM_H_ */
