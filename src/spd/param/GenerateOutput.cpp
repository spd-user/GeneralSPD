/**
 * GenerateOutput.cpp
 *
 * @date 2012/12/31
 * @author katsumata
 */

#include <stdexcept>
#include <cmath>
#include <algorithm>
#include "GenerateOutput.hpp"
#include "../output/AllOutput.hpp"
#include "../output/color/AllColor.hpp"

namespace spd {
namespace param {

using namespace std;

/**
 * コンストラクタ
 */
GenerateOutput::GenerateOutput() {

	using namespace spd::output;

	// 出力の作成

	// コンソール
	shared_ptr<Output> console = make_shared<ConsoleOutput>();
	auto consoleName = console->toString();
	transform(consoleName.begin(), consoleName.end(), consoleName.begin(), ::tolower);

	// プレイヤ数を数える
	shared_ptr<Output> number = make_shared<NumberOutput>();
	auto numberName = number->toString();
	transform(numberName.begin(), numberName.end(), numberName.begin(), ::tolower);

	// 利得を合計
	shared_ptr<Output> payoff = make_shared<PayoffOutput>();
	auto payoffName = payoff->toString();
	transform(payoffName.begin(), payoffName.end(), payoffName.begin(), ::tolower);

	// Image 系統の出力
	shared_ptr<color::ColorChooser> standard = make_shared<color::StandardColor>();
	shared_ptr<Output> image = make_shared<ImageOutput>(standard);
	auto imageName = image->toString();
	transform(imageName.begin(), imageName.end(), imageName.begin(), ::tolower);


	shared_ptr<color::ColorChooser> mem = make_shared<color::MembraneColor>();
	shared_ptr<Output> memimage = make_shared<ImageOutput>(mem);
	std::string memImageName = "membrane_image";
	transform(memImageName.begin(), memImageName.end(), memImageName.begin(), ::tolower);

	shared_ptr<color::ColorChooser> aff = make_shared<color::AffectColor>();
	shared_ptr<Output> affImage = make_shared<ImageOutput>(aff);
	std::string affImageName = "effect_image";
	transform(affImageName.begin(), affImageName.end(), affImageName.begin(), ::tolower);


	// binary の出力
	shared_ptr<Output> binary = make_shared<BinaryOutput>();
	auto binaryName = binary->toString();
	transform(binaryName.begin(), binaryName.end(), binaryName.begin(), ::tolower);

	// GEXF の出力
	shared_ptr<Output> gexf = make_shared<GEXFOutput>();
	auto gexfName = gexf->toString();
	transform(gexfName.begin(), gexfName.end(), gexfName.begin(), ::tolower);

	// Property の出力
	shared_ptr<Output> property = make_shared<PropertyOutput>();
	auto propertyName = property->toString();
	transform(propertyName.begin(), propertyName.end(), propertyName.begin(), ::tolower);



	map<string, shared_ptr<Output>> m {
		{consoleName, 	console},
		{numberName, 		number},
		{payoffName, 		payoff},
		{imageName, 		image},
		{memImageName, 	memimage},
		{affImageName, 	affImage},
		{binaryName, 		binary},
		{gexfName, 		gexf},
		{propertyName, 	property}
	};

	this->outputMap = m;
}

/*
 * 出力方法と各設定ステップから、タプルを作成
 *
 * デフォルトは、最後まで、毎ステップの出力
 * @param[in] output 出力方法と間隔を「:」で区切った文字列
 */
tuple<shared_ptr<output::Output>, int, int, int> GenerateOutput::generate(const string& output) const {

	shared_ptr<output::Output> outputMethod;
	int start;
	int end;
	int interval;

	vector<string> outputSetting = separateOutputString(output);

	try {
		auto itr = this->outputMap.find(outputSetting.front());
		if (itr == this->outputMap.end()) {
			throw std::exception();
		}
		outputMethod = itr->second;

		start = outputSetting.at(1).empty()? 0 : abs(std::stoi(outputSetting.at(1)));

		// これだけはマイナスでも構わない
		end = outputSetting.at(2).empty()? -1 : std::stoi(outputSetting.at(2));

		interval = outputSetting.at(3).empty()? 1 : abs(std::stoi(outputSetting.at(3)));
	} catch (std::exception& e) {
		throw std::invalid_argument("Could not generate output for " + output + "."
				" Please check settable outputs by using help.");
	}

	return std::make_tuple(outputMethod, start, end, interval);
}

/*
 * 文字列を、出力方法と初期ステップ、終了ステップ、間隔に分割する
 * @param[in] output 最低でも出力方法がある文字列
 * @return 分割されたタプル
 */
std::vector<std::string> GenerateOutput::separateOutputString(std::string output) const {

	std::vector<std::string> result;

	size_t next;
	size_t current = 0;

	do {
		next = output.find(":", current);

		auto cuttingMax = (next == std::string::npos)? output.size() : next;

		result.push_back(output.substr(current, cuttingMax - current));

		current = next + 1;
	} while (next != std::string::npos);

	// 要素数が4になるまで、空文字列を加える
	const int ELEMENT_NUM = 4;
	for (int i = result.size(); i < ELEMENT_NUM; ++i) {
		result.push_back(std::string());
	}

	return result;
}

/*
 * 方法と実態のマップを取得
 * @return 方法と実態のマップ
 */
const std::map<std::string, std::shared_ptr<spd::output::Output>>& GenerateOutput::getOutputMap() const {
	return this->outputMap;
}

} /* namespace param */
} /* namespace spd */
