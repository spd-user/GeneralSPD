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
	~ImageOutput();

	/**
	 * ムーア近傍に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Moore& topology, spd::core::Space& space);

	/**
	 * ノイマン近傍に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Neumann& topology, spd::core::Space& space);

	/**
	 * 六角格子に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Hexagon& topology, spd::core::Space& space);

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

	// 色のタイプ
	static const int COLOR_TYPE = PNG_COLOR_TYPE_RGB;
	//const int COLOR_TYPE = PNG_COLOR_TYPE_GRAY;

	static const int BYTE_PER_PIXEL = (COLOR_TYPE == PNG_COLOR_TYPE_RGB) ? 3 : 1;


private:
	// 色定義
	static constexpr int RGB = 3;

	const png_byte BLACK[RGB] = {0, 0, 0};
	const png_byte GRAY[RGB] = {0x80, 0x80, 0x80};
	const png_byte WHITE[RGB] = {0xFF, 0xFF, 0xFF};

	const png_byte MAGENTA[RGB] = {0xFF, 0x00, 0xFF};
	const png_byte GREEN[RGB] = {0x00, 0x80, 0x00};


	// 配置定義
	const png_byte (&ALLD_COLOR)[RGB] = BLACK;
	const png_byte (&ALLC_COLOR)[RGB] = WHITE;
	const png_byte (&MEMBRANE_COLOR)[RGB] = (COLOR_TYPE == PNG_COLOR_TYPE_RGB) ? MAGENTA : GRAY;
	const png_byte (&C_COLOR)[RGB] = GREEN;


	// 接頭辞
	const std::string PREFIX = "/image/spd_image_";
	// 接尾辞
	const std::string SUFFIX = ".png";

	// 空間画像サイズ
	//unsigned char **image;
	png_bytepp image;

	// 確保している画像の一辺の長さ
	int imageSide;

	// セルサイズ
	int cellSize;

	// 画像の生成
	void createImageFile(const spd::topology::Lattice& lattice, spd::core::Space& space, bool isHexagonLattice);

	// pngファイルの作成
	void outputPngFile(spd::core::Space& space, int sizeX, int sizeY);

	// 盤面状態のイメージ化
	void writeSpace(
			const spd::topology::Lattice& lattice,
			spd::core::Space& space,
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
