/**
 * ParseParam.cpp
 *
 * @date 2012/12/29
 * @author katsumata
 */

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <boost/program_options.hpp>

#include "ParseParam.hpp"

// パーサ系
#include "Option.hpp"
#include "InitParser.hpp"
#include "NeighborParser.hpp"
#include "OutputParser.hpp"
#include "RandomParser.hpp"
#include "RuntimeParser.hpp"

#include "../Parameter.hpp"
#include "../NeighborhoodParameter.hpp"

#include "../../core/Strategy.hpp"
#include "../../core/maker/CommandLineBasedMaker.hpp"
#include "../../core/maker/GEXFBasedMaker.hpp"
#include "../../core/maker/DeserializedMaker.hpp"

#include "../../topology/Topology.hpp"

#include "../../output/FileSystemOperation.hpp"
#include "../../output/Compressor.hpp"

namespace po = boost::program_options;

using spd::core::Strategy;

namespace spd {
namespace param {
namespace parser {

void ParseParam::parse(int argc, char* argv[], Parameter& param) const {

	// 一般的なオプションの生成
	auto generalOpt = po::options_description("General options",
			LINE_LENGTH, MIN_DESCRIPTION_LENGTH);

	generalOpt.add_options()
		("config", 	po::value<std::string>(), 	"Loading the configurations from the specified file."
				"But, an option is overridden by command line.")
		("state", 		po::value<std::string>(), 	"Loading the states of all players from the specified"
				" gexf or mpac file or these gziped file.")
		("core", 		po::value<int>()->default_value(param.getCore()),
														"Thread count for this simulation.")
		("help,h", 									"Output a brief help message.");

	// 戦略用のオプション
	auto strategyOpt = po::options_description("Strategy",
			LINE_LENGTH, MIN_DESCRIPTION_LENGTH);

	strategyOpt.add_options()
		("strategy,s", po::value<std::vector<std::string>>(),
				"Interaction strategies. To also set an initial ratio of the strategy, input a colon,"
				" followed by a number.\n"
				"The both patterns below are recognized.\n\t"
				"-s c6D3:2 -s ddddDDd3 d9.\nNote this option is case-insensitive and default option.");

	po::positional_options_description p;
	p.add("strategy", -1);

	// それぞれの解析器の作成とパラメタ保持クラスの設定
	InitParser initP(param.getInitialParameter());
	RuntimeParser runtimeP(param.getRuntimeParameter());
	NeighborParser neiP(param.getNeighborhoodParameter());
	OutputParser outputP(param.getOutputParameter());
	RandomParser randomP(param.getRandomParameter());

	// オプションの結合
	generalOpt.add(strategyOpt)
		.add(*(initP.getOptions().get()))
		.add(*(runtimeP.getOptions().get()))
		.add(*(neiP.getOptions().get()))
		.add(*(outputP.getOptions().get()))
		.add(*(randomP.getOptions().get()));

	// オプションの値の読み込み先
	po::variables_map vm;
	try {

		po::store(po::command_line_parser(argc, argv).options(generalOpt).positional(p).run(), vm);

		// ヘルプがあったら、ヘルプ表示で終了
		if (vm.count("help")) {
			std::cout << generalOpt << std::endl;
			exit(EXIT_SUCCESS);
		}

		// コア数
		param.setCore(vm["core"].as<int>());

		// 状態ファイルの読み込み
		if (vm.count("state")) {
			std::string fileName = vm["state"].as<std::string>();
			std::string stateFile = fileName;

			spd::output::FileSystemOperation fso;
			if (fso.isFileType(stateFile, spd::output::compressor::COMPRESS_TYPE)) {
				stateFile.erase(stateFile.size() - spd::output::compressor::COMPRESS_TYPE.size());
				if (!spd::output::compressor::decompress(fileName, stateFile)) {
					std::cerr << "Please set the other state file. Could not decompress." << std::endl;
					exit(EXIT_FAILURE);
				}
			}

			std::shared_ptr<spd::core::PlayerMaker> pm;
			if (fso.isFileType(stateFile, ".gexf")) {
				pm = std::make_shared<spd::core::GEXFBasedMaker>(param, stateFile);
			} else {
				pm = std::make_shared<spd::core::DeserializedMaker>(param, stateFile);
			}


			param.setPlayerMaker(pm);
		} else {
			std::shared_ptr<spd::core::PlayerMaker> pm =
					std::make_shared<spd::core::CommandLineBasedMaker>(param);
			param.setPlayerMaker(pm);
		}

		initP.parse(argc, argv, generalOpt);
		runtimeP.parse(argc, argv, generalOpt);
		neiP.parse(argc, argv, generalOpt);
		outputP.parse(argc, argv, generalOpt);
		randomP.parse(argc, argv, generalOpt);

		// 状態を読み込まないときのみ
		if (!vm.count("state")) {

			// 戦略の読み込み
			if (!vm.count("strategy")) {
				throw po::required_option("strategy");
			}

			// 戦略のリストへの適用
			auto strategies = vm["strategy"].as<std::vector<std::string>>();

			// クラスタスタートがある場合は、戦略は2こ以上
			if (vm.count("lonely-cluster") && strategies.size() <= 1) {
				throw std::runtime_error("Input more than 2 strategies to set lonely-cluster option.");
			}

			for (std::string strategy : strategies) {
				std::transform(strategy.begin(), strategy.end(), strategy.begin(), ::toupper);
				auto strategyPair = parseStrategyOptionValue(strategy, param.getNeighborhoodParameter());

				param.addStrategy(strategyPair);
			}
		}
	} catch (const po::required_option& e) {
		std::cerr << e.what() << " from option: " << e.get_option_name() << std::endl;
		exit(EXIT_FAILURE);
	} catch (const po::multiple_occurrences& e) {
		std::cerr << e.what() << " from option: " << e.get_option_name() << std::endl;
		exit(EXIT_FAILURE);
	} catch (boost::filesystem::filesystem_error& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "Please check whether the file exists." << std::endl;
		exit(EXIT_FAILURE);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
 * コマンドラインの戦略引数から戦略と割合のペアを作る
 * @param strategyOptValue コマンドラインの戦略引数の値
 * @return 戦略と割合のペア
 */
std::pair<std::shared_ptr<Strategy>, int> ParseParam::parseStrategyOptionValue(
		const std::string& strategyOptValue,
		const std::shared_ptr<NeighborhoodParameter>& neiParam) const {

	auto colonPos = strategyOptValue.find(':');
	if (colonPos != strategyOptValue.rfind(':')) {
		throw std::invalid_argument("Could not set a string including two colons to strategy ["
				+ strategyOptValue + "].");
	}

	// 初期割合
	int ratio = 1;
	if (colonPos != std::string::npos) {
		ratio = std::abs(std::stoi(strategyOptValue.substr(colonPos + 1)));
	}

	auto strategy = strategyOptValue.substr(0, colonPos);

	auto minLen = neiParam->getTopology()->minStrategyLength(
			neiParam->getNeiborhoodRadius(NeighborhoodType::ACTION));

	auto maxLen = neiParam->getTopology()->maxStrategyLength(
			neiParam->getNeiborhoodRadius(NeighborhoodType::ACTION));

	std::pair<std::shared_ptr<Strategy>, int> result {
		std::make_shared<Strategy>(strategy.c_str(), minLen, maxLen),
		ratio
	};

	return result;
}

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
