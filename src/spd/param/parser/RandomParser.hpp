/**
 * RandomParser.hpp
 *
 * @date 2013/01/04
 * @author katsumata
 */

#ifndef RANDOMPARSER_H_
#define RANDOMPARSER_H_

#include <memory>
#include <boost/program_options.hpp>

namespace spd {
namespace param {
class RandomParameter;

namespace parser {

/**
 * RandomParameter 用のオプション解析を行うクラス
 */
class RandomParser {
public:

	/**
	 * オプション解析器の作成
	 * @param[in, out] randomParam 解析結果を保持するパラメタクラス
	 */
	RandomParser(const std::shared_ptr<RandomParameter>& randomParam);

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
	 * 乱数生成に係わるパラメタのオプション
	 */
	std::shared_ptr<boost::program_options::options_description> options;

	/**
	 * 解析結果を保持するパラメタクラス
	 */
	const std::shared_ptr<RandomParameter>& rp;

};

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
#endif /* RANDOMPARSER_H_ */
