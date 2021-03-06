/**
 * PayoffOutput.cpp
 *
 * @date 2013/01/16
 * @author katsumata
 */

#include "PayoffOutput.hpp"

#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "../core/Space.hpp"
#include "../core/Strategy.hpp"
#include "../core/Player.hpp"
#include "../param/Parameter.hpp"
#include "../param/OutputParameter.hpp"
#include "FileSystemOperation.hpp"

namespace spd {
namespace output {

/*
 * 空間の情報を出力する
 * @param[in] space 空間
 */
std::pair<std::string, bool> PayoffOutput::output(spd::core::Space& space) {

	auto strategyList = space.getParameter().getStrategyList();
	int strategyListSize = strategyList.size();

	std::vector<double> countList(strategyListSize * 2, 0.0);

	// 数える
	for (auto player : space.getPlayers()) {
		for (int i = 0; i < strategyListSize; ++i) {
			if (strategyList.at(i).first->equals(*(player->getStrategy()))) {
				// 行動によって数え分ける
				if (player->getAction() == Action::ACTION_C) {
					countList.at(i * 2 + static_cast<int>(Action::ACTION_C)) += player->getScore();
				} else if (player->getAction() == Action::ACTION_D) {
					countList.at(i * 2 + static_cast<int>(Action::ACTION_D)) += player->getScore();
				}
				break;
			}
			if (i == strategyListSize -1) {
				throw std::runtime_error("Could not find a player's strategy from the strategy list.");
			}
		}
	}

	// 出力
	*(this->outputFile.get()) << std::setw(5) << std::setfill('0') << space.getStep() << ":<" <<
			strategyList.front().first->getShortStrategy() << "-C:" <<
			std::fixed << countList.front() << ">,<" <<
			strategyList.front().first->getShortStrategy() << "-D:" <<
			std::fixed << countList.at(1) << ">";

	for (int i = 1; i < strategyListSize; ++i) {
		*(this->outputFile.get()) << ",<" <<
				strategyList.at(i).first->getShortStrategy() << "-C:" << std::fixed <<
				countList.at(i * 2 +  static_cast<int>(Action::ACTION_C)) << ">,<" <<
				strategyList.at(i).first->getShortStrategy() << "-D:" << std::fixed <<
				countList.at(i * 2 +  static_cast<int>(Action::ACTION_D)) << ">";
	}
	*(this->outputFile.get()) << std::endl;

	return std::pair<std::string, bool> {std::string(""), false};
}


/*
 * 初期化
 * @param[in, out] space 空間
 * @param[in, out] param パラメタ
 */
void PayoffOutput::init(spd::core::Space& space, spd::param::Parameter& param) {
	std::ostringstream simCount;
	simCount.setf(std::ios::right);
	simCount.fill('0');
	simCount.width(3); // 桁数は3
	simCount << space.getSimCount();

	if (this->outputFile->is_open()) {
		this->outputFile->close();
	}

	std::string filename;
	filename = param.getOutputParameter()->getDirectory() + PREFIX + simCount.str() + SUFFIX;

	// ディレクトリの作成
	if (space.getSimCount() == 0) {
		FileSystemOperation fso;
		if (!fso.createDirectory(filename) ) {
			throw std::runtime_error("Could not create a directory for Payoff Output.");
		}
	}

	this->outputFile.reset(new std::ofstream());
	this->outputFile->open(filename.c_str(), std::ios::out);
	if (this->outputFile->fail()) {
		throw std::runtime_error("Could not open a file (" + filename + ").");
	}

	param.showParameter(*(this->outputFile.get()));
}


} /* namespace output */
} /* namespace spd */
