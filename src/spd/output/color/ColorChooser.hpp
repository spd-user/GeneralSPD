/**
 * ColorChooser.hpp
 *
 * @date 2014/01/29
 * @author katsumata
 */
#ifndef COLORCHOOSER_HPP_
#define COLORCHOOSER_HPP_

#include <string>
#include <png.h>
#include <memory>

namespace spd {
namespace core {
class Player;
}
namespace output {
namespace color {

/**
 * 色の選択を表すクラス
 */
class ColorChooser {

public:
	/**
	 * デストラクタ
	 */
	virtual ~ColorChooser(){}

	/**
	 * 色の選択
	 * @param 対象のプレイヤ
	 * @param 色のタイプ(PNG_COLOR_TYPE_RGB_ALPHA か PNG_COLOR_TYPE_GRAY_ALPHA)
	 */
	virtual const png_byte* chooseColor(
			const std::shared_ptr<spd::core::Player> player,
			int colorType) const = 0;

	/**
	 * 色のタイプ名を返す
	 * @return 色のタイプ名
	 */
	virtual const std::string colorTypeNmae() const = 0;

protected:


	/**
	 * RGBAに必要な情報の種類
	 */
	static constexpr int RGBA = 4;

	// 色指定
	const png_byte BLACK[RGBA] = {0, 0, 0, 0xFF}; /**< 黒色 */
	const png_byte GRAY[RGBA] = {0x80, 0x80, 0x80, 0xFF}; /**< 灰色 */
	const png_byte WHITE[RGBA] = {0xFF, 0xFF, 0xFF, 0xFF}; /**< 白色 */

	const png_byte DARK_GRAY[RGBA] = {0xA9, 0xA9, 0xA9, 0xFF}; /**< 濃い灰色 */
	const png_byte LIGHT_GRAY[RGBA] = {0xD3, 0xD3, 0xD3, 0xFF}; /**< 薄い灰色 */

	const png_byte MAGENTA[RGBA] = {0xFF, 0x00, 0xFF, 0xFF}; /**< マゼンタ */
	const png_byte GREEN[RGBA] = {0x00, 0x80, 0x00, 0xFF}; /**< 緑 */

	const png_byte VERMILION[RGBA] = {0xEF, 0x45, 0x4A, 0xFF}; /**< 朱色 */
	const png_byte AMBER[RGBA] = {0xFF, 0x7E, 0x00, 0xFF}; /**< 琥珀色 */
	const png_byte ORANGE[RGBA] = {0xFF, 0xB7, 0x4C, 0xFF}; /**< だいだい色 */
	const png_byte ULTRAMARINE[RGBA] = {0x46, 0x5D, 0xAA, 0xFF}; /**< 群青色(瑠璃色) */

};

} /* namespace color */
} /* namespace output */
} /* namespace spd */

#endif /* COLORCHOOSER_HPP_ */
