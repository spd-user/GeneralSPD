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
#include <png.h>

#include "../topology/AllTopology.hpp"
#include "../core/Space.hpp"
#include "../core/Player.hpp"
#include "../core/Strategy.hpp"
#include "../param/Parameter.hpp"
#include "../param/OutputParameter.hpp"

#include "FileSystemOperation.hpp"

namespace spd {
namespace output {

ImageOutput::ImageOutput() : image(nullptr), imageSide(0), cellSize(DEFAULT_CELL_SIZE) {}

ImageOutput::~ImageOutput() {

	// 開放
	for (int y = 0; y < imageSide; ++y) {
		delete[] image[y];
	}
	delete[] image;
}

/*
 * ムーア
 */
void ImageOutput::output(const spd::topology::Moore& topology, spd::core::Space& space) {
	createImageFile(topology, space, false);
}

/*
 * ノイマン近傍に即した出力
 * @param[in] topology 空間構造
 * @param[in] space 空間状態
 */
void ImageOutput::output(const spd::topology::Neumann& topology, spd::core::Space& space) {
	createImageFile(topology, space, false);
}

/*
 * 六角格子に即した出力
 * @param[in] topology 空間構造
 * @param[in] space 空間状態
 */
void ImageOutput::output(const spd::topology::Hexagon& topology, spd::core::Space& space) {
	createImageFile(topology, space, true);
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

	std::cerr << "Could not output on a console without a lattice.\n"
			"For making a image, please use gephi.\n";
}

/*
 * 初期化ではなし
 * @param[in, out] space 空間
 * @param[in, out] param パラメタ
 */
void ImageOutput::init(spd::core::Space& space, spd::param::Parameter& param) {

	std::string filename (param.getOutputParameter()->getDirectory() + PREFIX + SUFFIX);

	// ディレクトリの作成
	FileSystemOperation fso;
	if (!fso.createDirectory(filename) ) {
		throw std::runtime_error("Could not create a directory for Image Output.");
	}

	// セルサイズの設定
	cellSize = param.getOutputParameter()->getCellSize();
}


// 画像を生成する実体
void ImageOutput::createImageFile(
		const spd::topology::Lattice& lattice,
		spd::core::Space& space,
		bool isHexagonLattice) {

	// 一辺の長さ
	auto sideY = lattice.getSide() * cellSize;
	auto sideX = sideY;

	// 六角ならしっかりチェック
	if (isHexagonLattice) {
		// セルサイズを偶数にする
		cellSize = (cellSize % 2 == 0) ? cellSize : cellSize + 1;

		sideY = lattice.getSide() * cellSize;
		sideX = sideY + (cellSize / 2);
	}


	// サイズが変わっていたら、リセット
	if (image != nullptr && imageSide != sideY) {
		// 開放
		for (int y = 0; y < imageSide; ++y) {
			delete[] image[y];
		}
		delete[] image;

		image = nullptr;
	}

	// 領域がない場合確保
	if (image == nullptr) {

		// 確保
		image = new unsigned char*[sideY];
		for (int y = 0; y < sideY; ++y) {
			image[y] = new unsigned char[sideY];
		}
		imageSide = sideY;
	}

	// 盤面のイメージ作成
	writeSpace(lattice, space, isHexagonLattice);
	
	// 画像出力
	outputPngFile(space, sideX, sideY);
}

// png ファイルの作成
void ImageOutput::outputPngFile(
		spd::core::Space& space, int sizeX, int sizeY) {

	auto& param = space.getParameter();

	//(ディレクトリ)/image/spd_image_(3桁のsim)_(5桁のステップ数).png
	std::ostringstream simCount, step;
	setZeroPadding(simCount, 3, space.getSimCount());

	setZeroPadding(step, 5, space.getStep());

	std::string filename (param.getOutputParameter()->getDirectory() +
			PREFIX + simCount.str() + "_" + step.str() + SUFFIX);

	// ファイルを開く
	FILE *fp;
	fp = fopen(filename.c_str(), "w");
	if (fp == NULL) {
		std::runtime_error("Could not file create or open.\nFile name is " + filename);
	}

	// png_ptr構造体を確保・初期化
	auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		std::runtime_error("Cannot create png_ptr struct.");
	}

	// png_infop構造体を確保・初期化
	auto info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		std::runtime_error("Cannot create info_ptr struct.");
	}

	// libpngにfpを教える
	png_init_io(png_ptr, fp);

	// HDRチャンク情報を設定
	png_set_IHDR(png_ptr, info_ptr, sizeX, sizeY, 8, PNG_COLOR_TYPE_GRAY,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

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
		const spd::topology::Lattice& lattice,
		spd::core::Space& space,
		bool isHexagonLattice) {
	
	auto& allPlayer = space.getPlayers();
	
	for (int y = 0, side = lattice.getSide(); y < side; ++y) {
		for (int x = 0; x < side; ++x) {
			
			auto player = allPlayer.at(y * side + x);
			
			if (player->getAction() == Action::ACTION_C) {
				// C プレイヤ
				writePlayer(x, y, C_COLOR, isHexagonLattice);
			} else {
				// D プレイヤ
				if (player->getStrategy()->isAll(Action::ACTION_D)) {
					// all-D 戦略
					writePlayer(x, y, D_COLOR, isHexagonLattice);
				} else {
					writePlayer(x, y, KD_COLOR, isHexagonLattice);
				}
			}
		}
	}
}

// 1プレイヤを書く
void ImageOutput::writePlayer(
		int x, int y,
		unsigned char color, bool isHexagonLattice) {

	int dx = 0;
	// 六角の奇数番目(偶数行)は半分ずらす
	if (isHexagonLattice && y % 2 == 1) {
		// 六角のときは既に偶数のセルサイズにしてある
		dx = cellSize / 2;
	}
	
	/* 1セルを描く */
	for (int y0 = y * cellSize, yMax = y * cellSize + cellSize; y0 < yMax; ++y0) {
		for (int x0 = x * cellSize, xMax = x * cellSize + cellSize; x0 < xMax; ++x0) {
			image[y0][x0 + dx] = color;
		}
	}
}



} /* namespace output */
} /* namespace spd */
