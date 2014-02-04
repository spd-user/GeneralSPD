/**
 * MembraneColor.cpp
 *
 * @date 2014/01/30
 * @author katsumata
 */
#include "MembraneColor.hpp"

#include <stdexcept>

#include "../../core/Player.hpp"
#include "../../core/Strategy.hpp"
#include "../../core/Property.hpp"

#include "../../rule/property/MembraneDetectRule.hpp"

namespace spd {
namespace output {
namespace color {


/*
 * 色の選択
 * @param 対象のプレイヤ
 * @param 色のタイプ(PNG_COLOR_TYPE_RGB_ALPHA か PNG_COLOR_TYPE_GRAY_ALPHA)
 */
const png_byte* MembraneColor::chooseColor(
		const std::shared_ptr<spd::core::Player> player,
		int colorType) const {

	const png_byte* result;

	// 配色定義
	// 膜を表す色
	const png_byte *MEMBRANE_COLOR = VERMILION;
	// All-Dを表す色
	const png_byte *ALLD_COLOR = BLACK;
	// All-Cを表す色
	const png_byte *ALLC_COLOR = WHITE;
	// All-DでないDプレイヤを表す色
	const png_byte *D_COLOR = ORANGE;
	// All-CでないCプレイヤを表す色
	const png_byte *C_COLOR = ULTRAMARINE;

	int propertyVal;
	try {
		propertyVal = player->getProperty("MemGroup").getValueAs<int>();
	} catch (std::invalid_argument& e) {
		throw std::runtime_error("Could not find MemGroup property.(mc)");
	}

	if (propertyVal == static_cast<int>(
			spd::rule::MembraneDetectRule::Group::COMBINE) ||
			propertyVal == static_cast<int>(
					spd::rule::MembraneDetectRule::Group::BOTH_SIDE)) {
		result =  MEMBRANE_COLOR;

	} else if (player->getStrategy()->isAll(Action::ACTION_C)) {
		// allC 戦略
		result = ALLC_COLOR;
	} else if (player->getStrategy()->isAll(Action::ACTION_D)) {
		// allD 戦略
		result = ALLD_COLOR;
	} else if (player->getAction() == Action::ACTION_C) {
		// c
		result = C_COLOR;
	} else {
		// d (not membrane)
		result = D_COLOR;
	}

	return result;
}


} /* namespace color */
} /* namespace output */
} /* namespace spd */
