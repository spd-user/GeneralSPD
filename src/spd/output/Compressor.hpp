/**
 * Compressor.hpp
 *
 * @date 2013/02/08
 * @author katsumata
 */

#ifndef COMPRESSOR_HPP_
#define COMPRESSOR_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>

namespace spd {
namespace output {
namespace compressor {

/**
 * ファイル圧縮の拡張子
 */
const std::string COMPRESS_TYPE = ".gz";

/**
 * ファイルの圧縮を行う
 * @param[in] inputFileName 圧縮するファイル名
 * @param[in] outputFileName 圧縮したファイル名
 * @return 圧縮が成功したかどうか
 * @retval 成功した場合 true
 * @retval 失敗した場合 false
 */
inline bool compress(std::string inputFileName, std::string outputFileName) {

	namespace io = boost::iostreams;

	std::ifstream inputFile(inputFileName, std::ios_base::in | std::ios_base::binary);

	// 失敗したらエラーメッセージでもどり、終了させない
	if (inputFile.fail()) {
		std::cerr << "Could not compress a file (" << inputFileName <<
				"), \nbecause this program could not open that."<< std::endl;
		return false;
	}

	try {

		std::ofstream outputFile(outputFileName, std::ios_base::out | std::ios_base::binary);

		io::filtering_istreambuf input;

		input.push(io::gzip_compressor());
		input.push(inputFile);

		boost::iostreams::copy(input, outputFile);
		return true;

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return false;
}

/**
 * ファイルの解凍を行う
 * @param[in] inputFileName 解凍するファイル名
 * @param[in] outputFileName 解凍したファイル名
 * @return 解凍が成功したかどうか
 * @retval 成功した場合 true
 * @retval 失敗した場合 false
 */
inline bool decompress(std::string inputFileName, std::string outputFileName) {
	namespace io = boost::iostreams;

	std::ifstream inputFile(inputFileName, std::ios_base::in | std::ios_base::binary);

	// 失敗したらエラーメッセージでもどり、終了させない
	if (inputFile.fail()) {
		std::cerr << "Could not decompress a file (" << inputFileName <<
				"), \nbecause this program could not open that."<< std::endl;
		return false;
	}
	try {
		std::ofstream outputFile(outputFileName, std::ios_base::out | std::ios_base::binary);

		io::filtering_istreambuf input;

		input.push(io::gzip_decompressor());
		input.push(inputFile);

		boost::iostreams::copy(input, outputFile);
		return true;

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return false;
}

} /* namespace compressor */
} /* namespace output */
} /* namespace spd */


#endif /* COMPRESSOR_HPP_ */
