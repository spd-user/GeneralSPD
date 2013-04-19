/**
 * OutputParser.cpp
 *
 * @date 2012/12/30
 * @author katsumata
 */

#include "OutputParser.hpp"

#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <tuple>
#include <cmath>

#include "Option.hpp"
#include "../OutputParameter.hpp"

namespace po = boost::program_options;

namespace spd {
namespace param {
namespace parser {

/*
 * コンストラクタ
 */
OutputParser::OutputParser(const std::shared_ptr<OutputParameter>& outParam) : op(outParam) {

	// 出力方法の設定
	std::unique_ptr<GenerateOutput> g(new GenerateOutput());
	this->generator = std::move(g);

	// オプションの作成
	this->options = std::make_shared<po::options_description>("Output value",
			LINE_LENGTH, MIN_DESCRIPTION_LENGTH);

	// オプションの設定
	// 出力オプション用の説明
	std::string outputDescription = "Output method. You can set the following pattern(s).\n"
			"If you want to control an output, also input a beginning and ending step and,"
			" generating interval as indicated in the following examples."
			"\n\t   -o method:begin:end+1:interval\n\n"
			"To use a default value, do not input anything, or input a colon for next optional value."
			"\n\t   -o method1:::interval -o method2:begin\n"
			"Default value\n\t  begin - first(0), end - final(-1), interval - every(1)\n"
			"Settable method\n";

	auto outputMap = generator->getOutputMap();
	for (auto output : outputMap) {
		outputDescription += "\t-- " + output.first + "\n";
	}
	outputDescription += "\nNote this option is case-insensitive.";

	options->add_options()
		("dir,d", 		po::value<std::string>()->default_value(op->getDirectory()),
				"A root directory to output files.")
		("output,o", 	po::value<std::vector<std::string> >(), outputDescription.c_str())
		("cell", 		po::value<int>()->default_value(op->getCellSize()), "Cell size. When you output the png image files, "
				"the player cell's size is this value (px).");
}

/*
 * オプションの解析を行う
 */
void OutputParser::parse(
		int argc,
		char* argv[],
		boost::program_options::options_description allOptions) const {

	// オプションの値の読み込み先
	po::variables_map vm;

	try {

		// パースした結果を vm に格納する
		po::store(po::parse_command_line(argc, argv, allOptions), vm);

		// コンフィグファイル設定
		// 結果をvmに格納する
		if (vm.count("config")) {
			std::string filename = vm["config"].as<std::string>();
			std::ifstream ifs(filename);
			po::store(po::parse_config_file(ifs, allOptions), vm);
		}

		// outputオプション
		if (vm.count("output")) {
			auto outputs = std::vector<std::tuple<std::shared_ptr<output::Output>, int, int, int>>();
			auto outputsString = vm["output"].as<std::vector<std::string>>();
			for (std::string outputString : outputsString) {
				try {
					auto output = generator->generate(outputString);
					outputs.push_back(output);
				} catch (std::invalid_argument& e) {
					std::cerr << e.what() << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			this->op->getOutputs() = outputs;
		}

		auto directory = vm["dir"].as<std::string>();
		if (directory.rfind('/') == directory.size()-1) {
			directory.erase(directory.rfind('/'));
		}
		this->op->setDirectory(directory);

		this->op->setCellSize(std::abs(vm["cell"].as<int>()));

	} catch (const boost::program_options::multiple_occurrences& e) {
		std::cerr << e.what() << " from option: " << e.get_option_name() << std::endl;
		throw std::exception();
	} catch (std::exception &e) {
		std::cout << e.what() << "\n";
		throw std::exception();
	}
}

/*
 * オプションを取得
 * @return オプション説明
 */
const std::shared_ptr<boost::program_options::options_description>& OutputParser::getOptions() const {
	return this->options;
}



} /* namespace parser */
} /* namespace param */
} /* namespace spd */
