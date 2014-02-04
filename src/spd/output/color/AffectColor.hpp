/**
 * AffectColor.hpp
 *
 * @date 2014/02/04
 * @author katsumata
 */
#ifndef AFFECTCOLOR_HPP_
#define AFFECTCOLOR_HPP_

#include "ColorChooser.hpp"

namespace spd {
namespace output {
namespace color {

/**
 * 膜影響を表すクラス
 */
class AffectColor : public spd::output::color::ColorChooser {

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
		return "affect";
	}


};

} /* namespace color */
} /* namespace output */
} /* namespace spd */

#endif /* AFFECTCOLOR_HPP_ */
