/**
 * NeighborParser.hpp
 *
 * @date 2012/12/29
 * @author katsumata
 */

#ifndef NEIGHBORPARSER_H_
#define NEIGHBORPARSER_H_

#include <memory>
#include <boost/program_options.hpp>
#include "../GenerateTopology.hpp"

namespace spd {
namespace param {
class NeighborhoodParameter;

namespace parser {

/**
 * NeighborhoodParameter 用のオプション解析を行うクラス
 */
class NeighborParser {
public:
	/**
	 * オプション解析器の作成
	 * @param[in, out] neighborParam 解析結果を保持するパラメタクラス
	 */
	NeighborParser(const std::shared_ptr<NeighborhoodParameter>& neighborParam);

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
	 * @return オプション
	 */
	const std::shared_ptr<boost::program_options::options_description>& getOptions() const;

private:

	/**
	 * 近傍に係わるパラメタのオプション
	 */
	std::shared_ptr<boost::program_options::options_description> options;

	/**
	 * 解析結果を保持するパラメタクラス
	 */
	const std::shared_ptr<NeighborhoodParameter>& np;

	/**
	 * 空間構造のジェネレータ
	 */
	std::unique_ptr<GenerateTopology> generator;
};

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
#endif /* NEIGHBORPARSER_H_ */
