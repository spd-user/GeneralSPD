/**
 * ImageOutput.cpp
 *
 * @date 2013/01/30
 * @author katsumata
 */

#include "ImageOutput.hpp"

#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include "../topology/AllTopology.hpp"
#include "../core/Space.hpp"
#include "../core/Player.hpp"
#include "../core/Strategy.hpp"
#include "../param/Parameter.hpp"
#include "../param/InitParameter.hpp"
#include "../param/OutputParameter.hpp"

#include "FileSystemOperation.hpp"

namespace spd {
namespace output {

ImageOutput::ImageOutput(std::shared_ptr<color::ColorChooser> c) : image(nullptr), imageSide(0), cellSize(DEFAULT_CELL_SIZE) {
	color = c;
	colorName = c->colorTypeNmae();
}

ImageOutput::~ImageOutput() {

	// 開放
	if (image != nullptr) {
		freeImage();
	}
}

/*
 * 二次元格子
 */
void ImageOutput::output(const spd::topology::Lattice& topology, spd::core::Space& space) {

	bool isHex = (typeid(topology) == typeid(spd::topology::Hexagon)) ? true : false;
	create2DImageFile(space, topology.getSide(), -1, isHex);
}

/*
 * 立体格子に即した出力
 * @param[in] topology 空間構造
 * @param[in] space 空間状態
 */
void ImageOutput::output(const spd::topology::Cube& topology, spd::core::Space& space) {
	int playerNum = space.getPlayers().size();
	auto side = std::cbrt(playerNum);
	side = (2.0 * side + playerNum / side / side) / 3.0; // ニュートン法の漸化式

	for (int level = 0; level < side; ++level) {
		create2DImageFile(space, side, level, false);
	}

}

/*
 * ネットワーク構造に即した出力
 * @note
 * 標準エラー出力へのエラー出力のみ
 *
 * @param[in] topology 空間構造
 * @param[in] space 空間状態
 */
void ImageOutput::output(const spd::topology::Network& topology, spd::core::Space& space) {

	// 始めのステップだけエラー表示
	if (space.getStep() == 0) {
		std::cerr << "\nCould not output on a console without a lattice.\n"
				"For making a image, please use gephi.\n";
	}
}

/*
 * 初期化
 * @param[in, out] space 空間
 * @param[in, out] param パラメタ
 */
void ImageOutput::init(spd::core::Space& space, spd::param::Parameter& param) {

	int sim = param.getInitialParameter()->getSimCount();

	// 総シミュレーション数の桁数
	int digit = (sim - 1) / 10 + 1;

	for (int i = 0; i < sim; ++i) {
		std::ostringstream simCount;
		setZeroPadding(simCount, digit, i);

		std::string filename (param.getOutputParameter()->getDirectory() +
				DIR + "/" + colorName + "/sim" + simCount.str() +  PREFIX + SUFFIX);

		// ディレクトリの作成
		FileSystemOperation fso;
		if (!fso.createDirectory(filename) ) {
			throw std::runtime_error("Could not create a directory for Image Output.");
		}
	}

	// セルサイズの設定
	cellSize = param.getOutputParameter()->getCellSize();
}


// 画像を生成する実体
void ImageOutput::create2DImageFile(
		spd::core::Space& space, int side,
		int level, bool isHexagonLattice) {

	// 一辺の長さ
	auto sideY = side * cellSize;
	auto sideX = sideY;

	// 六角ならセルサイズをチェック
	if (isHexagonLattice) {
		// セルサイズを偶数にする
		cellSize = (cellSize % 2 == 0) ? cellSize : cellSize + 1;

		sideY = side * cellSize;
		sideX = sideY + (cellSize / 2);
	}


	// 幅のサイズが変わっていたら、リセット
	if (image != nullptr && imageSide != sideY) {
		// 開放
		freeImage();
		image = nullptr;
	}

	// 領域がない場合確保
	if (image == nullptr) {

		// 確保
		image = new png_bytep[sideY];

		for (int y = 0; y < sideY; ++y) {
			image[y] = new png_byte[sideY * BYTE_PER_PIXEL];
		}
		imageSide = sideY;
	}

	// 盤面のイメージ作成
	writeSpace(space, side, level, isHexagonLattice);

	// 画像出力
	outputPngFile(space, sideX, sideY, level);
}

// png ファイルの作成
void ImageOutput::outputPngFile(
		spd::core::Space& space, int sizeX, int sizeY, int level) {

	auto& param = space.getParameter();

	// 総シミュレーションの桁数
	int simDigit = (param.getInitialParameter()->getSimCount() - 1) / 10 + 1;


	//(ディレクトリ)/image/sim(総数による桁数のsim)/spd_image_(3桁のsim)_(5桁のステップ数)[-(3桁の階層)].png
	std::ostringstream simDir, simCount, step, levelCount;
	setZeroPadding(simDir, simDigit, space.getSimCount());

	setZeroPadding(simCount, 3, space.getSimCount());

	setZeroPadding(step, 5, space.getStep());

	std::string levelString = "";
	if (level != -1) {
		setZeroPadding(levelCount, 4, level);
		levelString = "-" + levelCount.str();
	}
	std::string filename (param.getOutputParameter()->getDirectory() +
			DIR + "/" + colorName + "/sim" + simDir.str() +  PREFIX + simCount.str() + "_" + step.str() + levelString + SUFFIX);

	// ファイルを開く
	FILE *fp;
	fp = fopen(filename.c_str(), "w");
	if (fp == NULL) {
		throw std::runtime_error("Could not file create or open.\nFile name is " + filename);
	}

	// png_ptr構造体を確保・初期化
	auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		throw std::runtime_error("Cannot create png_ptr struct.");
	}

	// png_infop構造体を確保・初期化
	auto info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		throw std::runtime_error("Cannot create info_ptr struct.");
	}

	// libpngにfpを教える
	png_init_io(png_ptr, fp);

	// HDRチャンク情報を設定 (8 bit color depth)
	png_set_IHDR(png_ptr, info_ptr, sizeX, sizeY, 8, COLOR_TYPE,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// PNGファイルのヘッダを書き込む
	png_write_info(png_ptr, info_ptr);

	// 画像データを書き込む
	png_write_image(png_ptr, image);
	// 残りの情報を書き込む
	png_write_end(png_ptr, info_ptr);
	// 2つの構造体を解放
	png_destroy_write_struct(&png_ptr, &info_ptr);
	/* ファイルを閉じる */
	fclose(fp);
}

// 0 埋め
inline void ImageOutput::setZeroPadding(std::ostringstream& oss, int width, int val) {
	oss.str("");
	oss.clear(std::stringstream::goodbit);
	oss.setf(std::ios::right);
	oss.fill('0');
	oss.width(width);
	oss << val;

}

// 盤面状態のイメージ化
void ImageOutput::writeSpace(
		spd::core::Space& space,
		int side, int level,
		bool isHexagonLattice) {

	auto& allPlayer = space.getPlayers();


	// 指定したピクセル範囲を透過処理する
	auto transparentPixel = [this](int x0, int x1, int y0, int y1){
		for (int y = y0; y < y1; ++y) {
			for (int x = x0 * BYTE_PER_PIXEL; x < x1 * BYTE_PER_PIXEL; x += BYTE_PER_PIXEL) {
				if (COLOR_TYPE == PNG_COLOR_TYPE_RGB_ALPHA) {
					image[y][x] = 0x0;
					image[y][x + 1] = 0x0;
					image[y][x + 2] = 0x0;
					image[y][x + 3] = 0x0; // 透明
				} else {
					image[y][x] = 0x0;
					image[y][x + 1] = 0x0; // 透明
				}
			}
		}
	};


	const png_byte* color;


	// z軸の値を構造座標へ変換
	int zPos = (level == -1) ? 0 : level * side * side;

	for (int y = 0; y < side; ++y) {

		// 必ず、両端サイズ分は透過処理を行う
		transparentPixel(0, cellSize, y * cellSize, (y + 1) * cellSize); // 左端
		transparentPixel(imageSide - cellSize, imageSide, y * cellSize, (y + 1) * cellSize); // 右端

		for (int x = 0; x < side; ++x) {

			auto& player = allPlayer.at(zPos + y * side + x);

			color = this->color->chooseColor(player, COLOR_TYPE);

			writePlayer(x, y, color, isHexagonLattice);
		}
	}
}

// 1プレイヤを書く
void ImageOutput::writePlayer(
		int x, int y,
		const png_byte* color, bool isHexagonLattice) {

	int dx = 0;
	// 六角の奇数番目(偶数行)は半分ずらす
	if (isHexagonLattice && y % 2 == 1) {
		// 六角のときは既に偶数のセルサイズにしてある
		dx = (cellSize / 2) * BYTE_PER_PIXEL;
	}

	// 1セルを描く
	for (int y0 = y * cellSize, yMax = y * cellSize + cellSize; y0 < yMax; ++y0) {
		for (int x0 = x * cellSize * BYTE_PER_PIXEL, xMax = (x * cellSize  + cellSize) * BYTE_PER_PIXEL; x0 < xMax; x0 += BYTE_PER_PIXEL) {
			if (COLOR_TYPE == PNG_COLOR_TYPE_RGB_ALPHA) {
				image[y0][x0 + dx] = color[0];
				image[y0][x0 + dx + 1] = color[1];
				image[y0][x0 + dx + 2] = color[2];
				image[y0][x0 + dx + 3] = color[3];
			} else {
				// rgb -> grayscale
				image[y0][x0 + dx] = (2 * color[0] + 4 * color[1] + color[2]) / 7;
				image[y0][x0 + dx + 1] = color[3];
			}
		}
	}
}


} /* namespace output */
} /* namespace spd */
