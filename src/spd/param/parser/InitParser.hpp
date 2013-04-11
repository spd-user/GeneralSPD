/**
 * InitParser.hpp
 *
 * @date 2012/12/21
 * @author katsumata
 */

#ifndef INITPARSER_H_
#define INITPARSER_H_

#include <memory>
#include <boost/program_options.hpp>
#include "../GenerateSpdRule.hpp"

namespace spd {
namespace param {
class InitParameter;

namespace parser {

/**
 * InitParameter 用のオプション解析を行うクラス
 */
class InitParser {

public:

	/**
	 * オプション解析器の作成
	 * @param[in, out] initParam 解析結果を保持するパラメタクラス
	 */
	InitParser(const std::shared_ptr<InitParameter>& initParam);

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
	 * 初期化のみに係わるパラメタのオプション
	 */
	std::shared_ptr<boost::program_options::options_description> options;

	/**
	 * 解析結果を保持するパラメタクラス
	 */
	const std::shared_ptr<InitParameter>& ip;

	/**
	 * ルールのジェネレータ
	 */
	std::unique_ptr<GenerateSpdRule> generator;

	/**
	 * コマンドラインのプロパティ引数からプロパティの名前と初期値のペアを作る
	 * @param propertyOptValue コマンドラインのプロパティ引数の値
	 * @return プロパティの名前と初期値のペア
	 */
	std::pair<std::string, std::string> parsePropertyOptionValue(
			const std::string& propertyOptValue) const;

	/**
	 * 使用可能メモリ量を測定する
	 * @return 使用可能メモリ量
	 */
	unsigned long measureAvailableMemory() const;

};

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
#endif /* INITPARSER_H_ */
