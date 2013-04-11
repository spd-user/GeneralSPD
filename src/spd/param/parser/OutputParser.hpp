/**
 * OutputParser.hpp
 *
 * @date 2012/12/30
 * @author katsumata
 */

#ifndef OUTPUTPARSER_H_
#define OUTPUTPARSER_H_

#include <memory>
#include <boost/program_options.hpp>
#include "../GenerateOutput.hpp"

namespace spd {
namespace param {
class OutputParameter;

namespace parser {

/**
 * OutputParamet 用のオプション解析を行うクラス
 */
class OutputParser {
public:
	/**
	 * オプション解析器の作成
	 * @param[in, out] outParam 解析結果を保持するパラメタクラス
	 */
	OutputParser(const std::shared_ptr<OutputParameter>& outParam);

	/**
	 * オプションの解析を行う
	 * @param[in] argc コマンドライン引数
	 * @param[in] argv コマンドライン引数値
	 * @param[in] allOptions すべてのオプション
	 */
	void parse(
			int argc,
			char* argv[],
			boost::program_options::options_description allOptions) const;

	/**
	 * オプションを取得
	 * @return オプション説明
	 */
	const std::shared_ptr<boost::program_options::options_description>& getOptions() const;

private:

	/**
	 * 出力に係わるパラメタのオプション
	 */
	std::shared_ptr<boost::program_options::options_description> options;

	/**
	 * 解析結果を保持するパラメタクラス
	 */
	const std::shared_ptr<OutputParameter>& op;

	/**
	 * 出力方法のジェネレータ
	 */
	std::unique_ptr<GenerateOutput> generator;
};

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
#endif /* OUTPUTPARSER_H_ */
