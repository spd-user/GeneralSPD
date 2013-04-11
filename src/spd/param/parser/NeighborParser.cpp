/**
 * NeighborParser.cpp
 *
 * @date 2012/12/29
 * @author katsumata
 */

#include "NeighborParser.hpp"

#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Option.hpp"
#include "../NeighborhoodParameter.hpp"
#include "../../topology/Topology.hpp"

namespace po = boost::program_options;

namespace spd {
namespace param {
namespace parser {

/*
 * オプション解析器の作成
 */
NeighborParser::NeighborParser(const std::shared_ptr<NeighborhoodParameter>& neighborParam) : np(neighborParam) {

	// 構造ジェネレータの設定
	std::unique_ptr<GenerateTopology> g(new GenerateTopology());
	this->generator = std::move(g);

	// オプションの作成
	this->options = std::make_shared<po::options_description>("Neighborhood value",
			LINE_LENGTH, MIN_DESCRIPTION_LENGTH);

	// オプションの設定
	// 空間構造オプション用の説明
	std::string topologyDescription = "Space topology. You can set the following topology(ies).\n";
	auto topologyMap = generator->getTopologyMap();
	for (auto topology : topologyMap) {
		topologyDescription += "\t-- " + topology.first + "\n";
	}
	topologyDescription += "Note this option is case-insensitive.";

	options->add_options()
		("neighbor-radius,n", po::value<int>()->default_value(np->getNeiborhoodRadius(NeighborhoodType::ACTION)),
				"All of the radius are set to NUMBER."
				" But if a specific option for a radius is used, this option is overridden.")
		("action-radius,A", po::value<int>(),
				"Neighborhood radius that is used when an action update phase. This option overrides a 'neighbor' option.")
		("game-radius,G", po::value<int>(),
				"Neighborhood radius that is used when an game phase. This option overrides a 'neighbor' option.")
		("strategy-update-radius,U", po::value<int>(),
				"Neighborhood radius that is used when an strategy update phase. This option overrides a 'neighbor' option.")
		("topology,t", po::value<std::string>()->default_value(np->getTopology()->toString()), topologyDescription.c_str());

}

/*
 * オプションの解析を行う
 * @param argc コマンドライン引数
 * @param argv コマンドライン引数値
 */
void NeighborParser::parse(
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

		// 近傍オプションの設定
		auto neighborType = NeighborhoodType::ACTION;
		if (vm.count("action-radius")) {
			this->np->setNeiborhoodRadius(neighborType, std::abs(vm["action-radius"].as<int>()));
		} else {
			this->np->setNeiborhoodRadius(neighborType, std::abs(vm["neighbor-radius"].as<int>()));
		}

		neighborType = NeighborhoodType::GAME;
		if (vm.count("game-radius")) {
			this->np->setNeiborhoodRadius(neighborType, std::abs(vm["game-radius"].as<int>()));
		} else {
			this->np->setNeiborhoodRadius(neighborType, std::abs(vm["neighbor-radius"].as<int>()));
		}

		neighborType = NeighborhoodType::STRATEGY;
		if (vm.count("strategy-update-radius")) {
			this->np->setNeiborhoodRadius(neighborType, std::abs(vm["strategy-update-radius"].as<int>()));
		} else {
			this->np->setNeiborhoodRadius(neighborType, std::abs(vm["neighbor-radius"].as<int>()));
		}

		// 空間構造
		try {
			std::string generateTopology = vm["topology"].as<std::string>();
			auto topology = generator->generate(generateTopology);
			this->np->setTopology(topology);
		} catch (std::invalid_argument& e) {
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}

	} catch (const boost::program_options::multiple_occurrences& e) {
		std::cerr << e.what() << " from option: " << e.get_option_name() << std::endl;
		throw std::exception();
	} catch (std::exception &e) {
		std::cout << e.what() << "\n";
		throw std::exception();
	}
}

/**
 * オプションを取得
 */
const std::shared_ptr<boost::program_options::options_description>& NeighborParser::getOptions() const {
	return this->options;
}

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
