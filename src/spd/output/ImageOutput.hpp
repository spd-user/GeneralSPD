/**
 * ImageOutput.hpp
 *
 * @date 2013/01/30
 * @author katsumata
 */

#ifndef IMAGEOUTPUT_H_
#define IMAGEOUTPUT_H_

#include <png.h>

#include "OutputVisitor.hpp"

namespace spd {
namespace output {

/**
 * 空間状態の画像出力を行うクラス
 */
class ImageOutput: public spd::output::OutputVisitor {

public:

	/*
	 * デフォルトのセルサイズ
	 */
	static const int DEFAULT_CELL_SIZE = 4;

	/**
	 * コンストラクタ
	 */
	ImageOutput();

	/**
	 * デストラクタ
	 */
	virtual ~ImageOutput();

	/**
	 * 二次元格子に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Lattice& topology, spd::core::Space& space);

	/**
	 * 立体格子に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Cube& topology, spd::core::Space& space);

	/**
	 * ネットワーク構造に即した出力
	 * @note
	 * 標準エラー出力へのエラー出力のみ
	 *
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Network& topology, spd::core::Space& space);

	/**
	 * 初期化
	 * @param[in, out] space 空間
	 * @param[in, out] param パラメタ
	 */
	void init(spd::core::Space& space, spd::param::Parameter& param);

	/**
	 * 出力方法名の出力
	 * @return 出力方法名
	 */
	std::string toString() const {
		return "image";
	}

private:

	/**
	 * 色のタイプ(RGBA または、GRAY_ALPHA)
	 */
	static const int COLOR_TYPE = PNG_COLOR_TYPE_RGB_ALPHA;
#ifdef GRAY
	COLOR_TYPE = PNG_COLOR_TYPE_GRAY_ALPHA;
#endif

	/**
	 * 1ピクセル辺りに必要なバイト数
	 */
	static const int BYTE_PER_PIXEL = (COLOR_TYPE == PNG_COLOR_TYPE_RGB_ALPHA) ? 4 : 2;

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


	// 配色定義
	const png_byte *ALLD_COLOR = BLACK; /**< All-Dを表す色 */
	const png_byte *ALLC_COLOR = WHITE; /**< All-Cを表す色 */
	const png_byte *D_COLOR = (COLOR_TYPE == PNG_COLOR_TYPE_RGB_ALPHA) ? MAGENTA : DARK_GRAY; /**< All-DでないDプレイヤを表す色 */
	const png_byte *C_COLOR = (COLOR_TYPE == PNG_COLOR_TYPE_RGB_ALPHA) ? GREEN : LIGHT_GRAY; /**< All-CでないCプレイヤを表す色 */


	/*
	 * 出力ディレクトリ名
	 */
	const std::string DIR = "/image";

	/**
	 * ファイルの接頭辞
	 */
	const std::string PREFIX = "/spd_image_";

	/*
	 * ファイルの接尾辞
	 */
	const std::string SUFFIX = ".png";

	/*
	 * 空間画像
	 */
	png_bytepp image;

	/*
	 *  確保している画像の幅
	 */
	int imageSide;

	/*
	 * セルサイズ
	 */
	int cellSize;

	/**
	 * 画像の生成
	 * @param space 空間
	 * @param side 一辺
	 * @param level 階層(-1 は階層がない、二次元格子)
	 * @param isHexagonLattice 六角格子かどうか
	 */
	void create2DImageFile(spd::core::Space& space, int side,
			int level, bool isHexagonLattice);

	// pngファイルの作成
	void outputPngFile(spd::core::Space& space, int sizeX, int sizeY, int level);

	// 盤面状態のイメージ化
	void writeSpace(
			spd::core::Space& space,
			int side, int level,
			bool isHexagonLattice);

	// 1プレイヤ分を指定した色で書く
	void writePlayer(
			int x, int y,
			const png_byte* color,
			bool isHexagonLattice);

	// 0 埋め用のマニピュレータ
	void setZeroPadding(std::ostringstream& oss, int width, int val);

	// 画像領域の開放
	void freeImage() {
		for (int y = 0; y < imageSide; ++y) {
			delete[] image[y];
		}
		delete[] image;
	}
};

} /* namespace output */
} /* namespace spd */
#endif /* IMAGEOUTPUT_H_ */
