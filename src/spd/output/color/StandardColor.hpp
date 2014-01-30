/**
 * StandardColor.hpp
 *
 * @date 2014/01/29
 * @author katsumata
 */
#ifndef STANDARDCOLOR_HPP_
#define STANDARDCOLOR_HPP_

#include "ColorChooser.hpp"

namespace spd {
namespace output {
namespace color {


/**
 * 標準の色を表すクラス
 */
class StandardColor: public spd::output::color::ColorChooser {

public:

	/**
	 * 色の選択
	 * @param 対象のプレイヤ
	 * @param 色のタイプ(PNG_COLOR_TYPE_RGB_ALPHA か PNG_COLOR_TYPE_GRAY_ALPHA)
	 */
	const png_byte* chooseColor(
			const std::shared_ptr<spd::core::Player> player,
			int colorType) const;

	/**
	 * 色のタイプ名を返す
	 * @return 色のタイプ名
	 */
	const std::string colorTypeNmae() const {
		return "standard";
	}

};

} /* namespace color */
} /* namespace output */
} /* namespace spd */

#endif /* STANDARDCOLOR_HPP_ */
