/**
 * StandardColor.cpp
 *
 * @date 2014/01/29
 * @author katsumata
 */
#include "StandardColor.hpp"

#include "../../core/Player.hpp"
#include "../../core/Strategy.hpp"

namespace spd {
namespace output {
namespace color {


/*
 * 色の選択
 * @param 対象のプレイヤ
 * @param 色のタイプ(PNG_COLOR_TYPE_RGB_ALPHA か PNG_COLOR_TYPE_GRAY_ALPHA)
 */
const png_byte* StandardColor::chooseColor(
		const std::shared_ptr<spd::core::Player> player,
		int colorType) const {

	const png_byte* result;

	// 配色定義
	// All-Dを表す色
	const png_byte *ALLD_COLOR = BLACK;
	// All-Cを表す色
	const png_byte *ALLC_COLOR = WHITE;
	// All-DでないDプレイヤを表す色
	const png_byte *D_COLOR = (colorType == PNG_COLOR_TYPE_RGB_ALPHA) ? VERMILION : DARK_GRAY;
	// All-CでないCプレイヤを表す色
	const png_byte *C_COLOR = (colorType == PNG_COLOR_TYPE_RGB_ALPHA) ? ULTRAMARINE : LIGHT_GRAY;


	if (player->getStrategy()->isAll(Action::ACTION_C)) {
		// allC 戦略
		result =  ALLC_COLOR;
	} else if (player->getStrategy()->isAll(Action::ACTION_D)) {
		// allD 戦略
		result = ALLD_COLOR;
	} else if (player->getAction() == Action::ACTION_C) {
		// c
		result = C_COLOR;
	} else {
		// membarane
		result = D_COLOR;
	}

	return result;

}

} /* namespace color */
} /* namespace output */
} /* namespace spd */
