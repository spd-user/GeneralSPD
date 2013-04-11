/**
 * GenerateSpdRule.hpp
 *
 * @date 2013/01/09
 * @author katsumata
 */

#ifndef GENERATESPDRULE_H_
#define GENERATESPDRULE_H_

#include <map>
#include <string>
#include <memory>

namespace spd {
namespace rule {
class SpdRule;
}
namespace param {

/**
 * STGPDのルール生成を行うクラス
 */
class GenerateSpdRule {
public:

	/**
	 * コンストラクタ
	 */
	GenerateSpdRule();

	/**
	 * 文字列から、ルールを作成する
	 * @param[in] spdRule ルール名
	 * @retval nullptr ルール名に対応する構造を作成できない場合
	 * @throw invalid_argument 文字列が空の場合または、ルール名に対応する構造を作成できない場合
	 */
	std::shared_ptr<spd::rule::SpdRule> generate(const std::string& spdRule) const;

	/**
	 * SPDルール名とSPDルールの対応を取得
	 */
	const std::map<std::string, std::shared_ptr<spd::rule::SpdRule>>& getSpdRuleMap() const;

private:

	/**
	 * SPDルール名とSPDルールを結びつけているmap
	 */
	std::map<std::string, std::shared_ptr<spd::rule::SpdRule>> spdRuleMap;
};

} /* namespace param */
} /* namespace spd */
#endif /* GENERATESPDRULE_H_ */
