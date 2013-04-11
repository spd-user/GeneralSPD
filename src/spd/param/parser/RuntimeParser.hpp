/**
 * RuntimeParser.hpp
 *
 * @date 2013/01/04
 * @author katsumata
 */

#ifndef RUNTIMEPARSER_H_
#define RUNTIMEPARSER_H_

#include <memory>
#include <boost/program_options.hpp>

namespace spd {
namespace param {
class RuntimeParameter;

namespace parser {

/**
 * RuntimeParameter 用のオプション解析を行うクラス
 */
class RuntimeParser {
public:

	/**
	 * オプション解析器の作成
	 * @param[in, out] runtimeParam 解析結果を保持するパラメタクラス
	 */
	RuntimeParser(const std::shared_ptr<RuntimeParameter>& runtimeParam);

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
	 * 実行時に変わる可能性が有るパラメタのオプション
	 */
	std::shared_ptr<boost::program_options::options_description> options;

	/**
	 * 解析結果を保持するパラメタクラス
	 */
	const std::shared_ptr<RuntimeParameter>& rp;

};

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
#endif /* RUNTIMEPARSER_H_ */
