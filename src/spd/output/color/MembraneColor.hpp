/**
 * MembraneColor.hpp
 *
 * @date 2014/01/30
 * @author katsumata
 */
#ifndef MEMBRANECOLOR_HPP_
#define MEMBRANECOLOR_HPP_

#include "ColorChooser.hpp"

namespace spd {
namespace output {
namespace color {

/**
 * 膜検知の色を表すクラス
 */
class MembraneColor: public spd::output::color::ColorChooser {
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
		return "membrane";
	}
};

} /* namespace color */
} /* namespace output */
} /* namespace spd */

#endif /* MEMBRANECOLOR_HPP_ */
