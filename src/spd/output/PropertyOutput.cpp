/**
 * PropertyOutput.cpp
 *
 * @date 2013/04/18
 * @author katsumata
 */

#include "PropertyOutput.hpp"

#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <map>

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
std::pair<std::string, bool> PropertyOutput::output(spd::core::Space& space) {


	auto& allPlayers = space.getPlayers();

	auto& properties = allPlayers.front()->getProperties();

	int outputFileIndex = 0;

	try {

		// プロパティ別に数える
		for (int i = 0, propNum = properties.size(); i < propNum; ++i) {
			auto& firstProp = properties.at(i);
			// 出力するもののみ
			if (firstProp.getType() != core::Property::OutputType::NOT) {

				// プロパティの情報
				std::map<std::string, int> propertyMap;

				// 分類するプロパティ
				if (firstProp.getType() == core::Property::OutputType::CLASSIFIABLE) {

					for (auto player : allPlayers) {

						auto propertyVal = player->getProperties().at(i).valueToString();

						auto result = propertyMap.insert(
								std::pair<std::string, int>(
										propertyVal, 1));
						if (!(result.second)) {
							propertyMap[propertyVal] += 1;
						}
					}
				} else if (firstProp.getType() == core::Property::OutputType::SPECIAL) {
					propertyMap = firstProp.getCountingMethod()->propOutput(allPlayers, i);
				}

				// 表示するものが無い場合は飛ばす
				if (propertyMap.size() <= 0) {
					continue;
				}

				auto& outputFile = *(outputFiles.at(outputFileIndex).get());

				// 計算結果を出力
				outputFile << std::setw(5) << std::setfill('0') << space.getStep() << ":<" <<
						propertyMap.begin()->first << ":" << propertyMap.begin()->second << ">";

				for (auto it = ++(std::begin(propertyMap)); it != std::end(propertyMap); it++){
					outputFile << ",<" <<
							it->first << ":" << it->second << ">";
				}
				outputFile << std::endl;

				outputFileIndex++;
			}
		}
	} catch (std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}

	return std::pair<std::string, bool> {std::string(""), false};
}


/*
 * 初期化
 * @param[in, out] space 空間
 * @param[in, out] param パラメタ
 */
void PropertyOutput::init(spd::core::Space& space, spd::param::Parameter& param) {

	std::ostringstream simCount;
	simCount.setf(std::ios::right);
	simCount.fill('0');
	simCount.width(3); // 3桁数
	simCount << space.getSimCount();

	for (auto& outputFile : outputFiles) {
		if (outputFile->is_open()) {
			outputFile->close();
		}
	}
	outputFiles.clear();

	// 先頭プレイヤのプロパティをみて、必要プロパティを見る
	auto& headPlayerProperties = space.getPlayers().front()->getProperties();

	for (auto& property : headPlayerProperties) {

		// 表示するプロパティのみ行う
		if (property.getType() != core::Property::OutputType::NOT) {

			std::string filename;
			filename = param.getOutputParameter()->getDirectory() + PREFIX
					+ property.getName() + "_" + simCount.str() + SUFFIX;

			FileSystemOperation fso;
			if (!fso.createDirectory(filename)) {
				throw std::runtime_error("Could not create a directory for Property Output.");
			}

			std::shared_ptr<std::ofstream> outputFile =
					std::make_shared<std::ofstream>(filename.c_str(), std::ios::out);

			if (outputFile->fail()) {
				throw std::runtime_error("Could not open a file (" + filename + ").");
			}

			outputFiles.push_back(outputFile);

			param.showParameter(*(outputFile.get()));
		}
	}
}


} /* namespace output */
} /* namespace spd */
