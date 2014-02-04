/**
 * AffectColor.cpp
 *
 * @date 2014/02/04
 * @author katsumata
 */
#include "AffectColor.hpp"

#include <stdexcept>

#include "../../core/Player.hpp"
#include "../../core/Strategy.hpp"
#include "../../core/Property.hpp"

#include "../../rule/property/AffectedPlayerRule.hpp"
#include "../../rule/property/MembraneDetectRule.hpp"

namespace spd {
namespace output {
namespace color {

using spd::rule::AffectedPlayerRule;


/*
 * 色の選択
 * @param 対象のプレイヤ
 * @param 色のタイプ(PNG_COLOR_TYPE_RGB_ALPHA か PNG_COLOR_TYPE_GRAY_ALPHA)
 */
const png_byte* AffectColor::chooseColor(
		const std::shared_ptr<spd::core::Player> player,
		int colorType) const {

	const png_byte* result;

	// 配色定義(優先順位は上)

	// 膜影響を表す色
	const png_byte *AFFECTED_COLOR = ULTRAMARINE;
	// 膜を表す色
	const png_byte *MEMBRANE_COLOR = VERMILION;
	// 膜の効果のない膜
	const png_byte *USELESS_COLOR = AMBER;
	// 無視する
	const png_byte *IGNORE_COLOR = BLACK;
	// 敵の影響
	const png_byte *ENEMY_COLOR = WHITE;
	//

	int memProp;
	int affProp;
	try {
		affProp = player->getProperty("Affect").getValueAs<int>();
		memProp = player->getProperty("MemGroup").getValueAs<int>();

	} catch (std::invalid_argument& e) {
		std::cerr << "Could not find MemGroup or Affect property.\nPlease use other rule." << std::endl;
		throw std::runtime_error("Could not find MemGroup or Affect property.");
	}


	if (affProp == static_cast<int>(AffectedPlayerRule::Affect::MEMBRANE)) {
		result = AFFECTED_COLOR;
	} else if (memProp == static_cast<int>(
			spd::rule::MembraneDetectRule::Group::COMBINE) ||
			memProp == static_cast<int>(
					spd::rule::MembraneDetectRule::Group::BOTH_SIDE)) {
		result =  MEMBRANE_COLOR;
	} else if (affProp == static_cast<int>(AffectedPlayerRule::Affect::ENEMY)) {
		result = ENEMY_COLOR;
	} else if (player->getStrategy()->isAll(Action::ACTION_D)) {
		result = IGNORE_COLOR;
	} else {
		result = USELESS_COLOR;
	}

	return result;
}

} /* namespace color */
} /* namespace output */
} /* namespace spd */
