/**
 * OutputParameter.cpp
 *
 * @date 2012/09/20
 * @author katsumata
 */

#include "OutputParameter.hpp"

#include "../output/Output.hpp"

#include "../output/ImageOutput.hpp" // cell size default

namespace spd {
namespace param {

/*
 * デフォルトコンストラクタ
 */
OutputParameter::OutputParameter() :
	directory(DEFAULT_DIR), cellSize(spd::output::ImageOutput::DEFAULT_CELL_SIZE) {
	// 出力方法は空
	outputs = std::vector<std::tuple<std::shared_ptr<output::Output>, int, int, int>>();
}

/*
 * getter/setter
 */
const std::string& OutputParameter::getDirectory() const {
	return directory;
}

void OutputParameter::setDirectory(const std::string& directory) {
	this->directory = directory;
}


std::vector<std::tuple<std::shared_ptr<output::Output>, int, int, int>>& OutputParameter::getOutputs(){
	return outputs;
}


void OutputParameter::showParameter(std::ostream& out) const {

	out << "dir = " << directory << "\n";

	if (outputs.size() != 0) {
		for (auto output : outputs) {
			out << "output = " << std::get<0>(output)->toString() << ":" << std::get<1>(output) <<
					 ":" << std::get<2>(output) <<  ":" << std::get<3>(output) << "\n";
		}
	} else {
		out << "# Do not output\n";
	}
}

} /* namespace param */
} /* namespace spd */
