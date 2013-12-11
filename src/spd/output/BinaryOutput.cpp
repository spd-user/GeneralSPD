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
#include "../param/InitParameter.hpp"
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


	// 総シミュレーションの桁数
	int simDigit = (space.getParameter().getInitialParameter()->getSimCount() - 1) / 10 + 1;

	//(ディレクトリ)/dump/sim(桁数を調整したsim)/spd_dump_(3桁のsim)_(5桁のステップ数).mpac
	std::ostringstream simDir, simCount, step;
	setZeroPadding(simDir, simDigit, space.getSimCount());
	setZeroPadding(simCount, 3, space.getSimCount());
	setZeroPadding(step, 5, space.getStep());

	std::string filename (space.getParameter().getOutputParameter()->getDirectory() +
			DIR + "/sim" + simDir.str() + PREFIX + simCount.str() + "_" + step.str() + SUFFIX);

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

	int sim = param.getInitialParameter()->getSimCount();

	// 総シミュレーション数の桁数
	int digit = (sim - 1) / 10 + 1;

	for (int i = 0; i < sim; ++i) {
		std::ostringstream simCount;
		setZeroPadding(simCount, digit, i);

		std::string filename (param.getOutputParameter()->getDirectory() +
				DIR + "/sim" + simCount.str() +  PREFIX + SUFFIX);

		// ディレクトリの作成
		FileSystemOperation fso;
		if (!fso.createDirectory(filename) ) {
			throw std::runtime_error("Could not create a directory for Gexf Output.");
		}
	}


	std::ofstream confFile (param.getOutputParameter()->getDirectory() +
			DIR + PREFIX + "config.conf");
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
