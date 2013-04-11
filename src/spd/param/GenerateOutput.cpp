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

namespace spd {
namespace param {

using namespace std;

/**
 * コンストラクタ
 */
GenerateOutput::GenerateOutput() {

	// 出力の作成

	// コンソール
	shared_ptr<spd::output::Output> console = make_shared<spd::output::ConsoleOutput>();
	auto consoleName = console->toString();
	transform(consoleName.begin(), consoleName.end(), consoleName.begin(), ::tolower);

	// プレイヤ数を数える
	shared_ptr<spd::output::Output> number = make_shared<spd::output::NumberOutput>();
	auto numberName = number->toString();
	transform(numberName.begin(), numberName.end(), numberName.begin(), ::tolower);

	// 利得を合計
	shared_ptr<spd::output::Output> payoff = make_shared<spd::output::PayoffOutput>();
	auto payoffName = payoff->toString();
	transform(payoffName.begin(), payoffName.end(), payoffName.begin(), ::tolower);

	// Image の出力
	shared_ptr<spd::output::Output> image = make_shared<spd::output::ImageOutput>();
	auto imageName = image->toString();
	transform(imageName.begin(), imageName.end(), imageName.begin(), ::tolower);

	// binary の出力
	shared_ptr<spd::output::Output> binary = make_shared<spd::output::BinaryOutput>();
	auto binaryName = binary->toString();
	transform(binaryName.begin(), binaryName.end(), binaryName.begin(), ::tolower);

	// GEXF 出力
	shared_ptr<spd::output::Output> gexf = make_shared<spd::output::GEXFOutput>();
	auto gexfName = gexf->toString();
	transform(gexfName.begin(), gexfName.end(), gexfName.begin(), ::tolower);


	map<string, shared_ptr<spd::output::Output>> m {
		{consoleName, console},
		{numberName, 	number},
		{payoffName, 	payoff},
		{imageName, 	image},
		{binaryName, 	binary},
		{gexfName, 	gexf}
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
 * 文字列を、出力方法と間隔に分割する
 * @param[in] output 出力方法と間隔の文字列
 * @return 方法と間隔のペア
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
