/**
 * Converter.hpp
 *
 * @date 2012/09/06
 * @author katsumata
 */

#ifndef CONVERTER_H_
#define CONVERTER_H_

#include <stdexcept>
#include "Action.hpp"

namespace spd {
namespace core {
namespace converter {

/**
 * 行動文字を受け取り、該当する行動列挙型に変換する
 * @param[in] action 行動文字
 * @return 行動文字の列挙型
 */
constexpr Action charToActoin(char action) {

	return (('C') == action || ('c') == action)? Action::ACTION_C :
			((('D') == action || ('d') == action)? Action::ACTION_D :
					Action::ACTION_UN);
}

/**
 * 行動列挙型を受け取り、該当する文字に変換する
 * @param[in] action 行動列挙型
 * @return 行動文字
 * @throw std::invalid_argument 変換できない場合
 */
constexpr char actionToChar(const Action& action) {

	return action == Action::ACTION_C ? 'C' :
			(action == Action::ACTION_D ? 'D' :
					throw std::invalid_argument("Could not convert to char from Action ENUM."));
}


} /* namespace converter */
} /* namespace core */
} /* namespace spd */

#endif /* CONVERTER_H_ */
