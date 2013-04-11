/**
 * BinaryOutput.cpp
 *
 * @date 2013/02/08
 * @author katsumata
 */

#include "BinaryOutput.hpp"

#include <iomanip>
#include <fstream>
#include "../core/Space.hpp"
#include "../param/Parameter.hpp"
#include "../param/OutputParameter.hpp"

#include "FileSystemOperation.hpp"

#include "../core/msgpack/SerializedSpace.hpp"

namespace spd {
namespace output {

/*
* 空間の情報を出力する
* @param[in, out] space 空間
*/
std::pair<std::string, bool> BinaryOutput::output(spd::core::Space& space) {

	//(ディレクトリ)/dump/spd_dump_(3桁のsim)_(5桁のステップ数).mpac
	std::ostringstream simCount, step;
	setZeroPadding(simCount, 3, space.getSimCount());
	setZeroPadding(step, 5, space.getStep());

	std::string filename (space.getParameter().getOutputParameter()->getDirectory() +
			PREFIX + simCount.str() + "_" + step.str() + SUFFIX);

	std::ofstream outputfile (filename);

	spd::core::serialize::SerializedSpace ss (space);

	msgpack::pack(&outputfile, ss);

	return std::pair<std::string, bool> {filename, true};
}

/*
* 初期化
*
* @par
* configファイルの作成
*
* @param[in, out] space 空間
* @param[in, out] param パラメタ
*/
void BinaryOutput::init(spd::core::Space& space, spd::param::Parameter& param) {

	std::string filename (param.getOutputParameter()->getDirectory() + PREFIX + "config.conf");

	// ディレクトリの作成
	FileSystemOperation fso;
	if (!fso.createDirectory(filename) ) {
		throw std::runtime_error("Could not create a directory for Gexf Output.");
	}

	std::ofstream confFile (filename);
	if (confFile.fail()) {
		throw std::runtime_error("Could not file open for dump config file.");
	}

	param.showParameter(confFile);
}

inline void BinaryOutput::setZeroPadding(std::ostringstream& oss, int width, int val) {
	oss.str("");
	oss.clear(std::stringstream::goodbit);
	oss.setf(std::ios::right);
	oss.fill('0');
	oss.width(width);
	oss << val;
}

} /* namespace output */
} /* namespace spd */
