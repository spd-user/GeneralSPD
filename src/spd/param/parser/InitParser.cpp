/**
 * InitParser.cpp
 *
 * @date 2012/12/21
 * @author katsumata
 */

#include "InitParser.hpp"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <fstream>
#include "Option.hpp"

#include "../InitParameter.hpp"
#include "../../core/Converter.hpp"
#include "../../rule/SpdRule.hpp"

namespace po = boost::program_options;

namespace spd {
namespace param {
namespace parser {

/*
 * コンストラクタ
 */
InitParser::InitParser(const std::shared_ptr<InitParameter>& initParam) : ip(initParam) {

	// デフォルトで使用するのは、使用可能メモリの 2/3 とする
	auto availableMemory = measureAvailableMemory() * 2 / 3;


	// 構造ジェネレータの設定
	std::unique_ptr<GenerateSpdRule> g(new GenerateSpdRule());
	this->generator = std::move(g);

	// オプションの作成
	this->options = std::make_shared<po::options_description>("Unchangeable value in the simulation",
			LINE_LENGTH, MIN_DESCRIPTION_LENGTH);

	// オプションの設定
	// ルールオプション用の説明
	std::string ruleDescription = "Rule of this STGPD. You can set the following rule(s).\n";
	auto ruleMap = generator->getSpdRuleMap();
		for (auto rule : ruleMap) {
			ruleDescription += "\t-- " + rule.first + "\n";
		}
		ruleDescription += "Note this option is case-insensitive.";

	options->add_options()
	 ("player-unit,u", 		po::value<int>()->default_value(ip->getPlayerNum()), "Number of player units.")
	 ("end-steps,e", 			po::value<int>()->default_value(ip->getEndStep()), "End steps.")
	 ("fixed-action,f", 		po::value<char>(),
			 "If set this option, set the all of the players' initial action to be ACTION.")
	 ("lonely-cluster,l", 	po::value<int>(), // ParseParam での戦略読み込みでも用いている
			 "If set this option, a cluster of a size NUMBER that has the first set "
			 "strategy is only located within the sea of other strategies.")
	 ("rule,r", 				po::value<std::string>()->default_value(ip->getSpdRule()->getName()),
			 ruleDescription.c_str())
	 ("memory", 				po::value<long int>()->default_value(availableMemory),
			 "Available memory for this simulation. If unset this option, "
			 "for this simulation, we use two thirds of a free memory.")
	 ("sim", 					po::value<int>()->default_value(ip->getSimCount()), "Number of simulation times.")
	 ("property,p", 			po::value<std::vector<std::string>>(),
			 "Property name and its initial value. Follow a name and the colon,"
			 " input an initial value or an initializing method.");
}

/*
 * 解析を行う
 */
void InitParser::parse(
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

		// 全ての値を、適用させる
		this->ip->setEndStep(std::abs(vm["end-steps"].as<int>()));

		if (vm.count("fixed-action")) {
			this->ip->setFixedAction(spd::core::converter::charToActoin(vm["fixed-action"].as<char>()));
			this->ip->setStartsInitialFixedAction(true);
		}

		if (vm.count("lonely-cluster")) {
			this->ip->setStartClusterSize(std::abs(vm["lonely-cluster"].as<int>()));
			this->ip->setStartsCluster(true);
		}

		this->ip->setMemory(std::abs(vm["memory"].as<long int>()));
		this->ip->setSimCount(std::abs(vm["sim"].as<int>()));
		this->ip->setPlayerNum(std::abs(vm["player-unit"].as<int>()));

		// ルール
		try {
			std::string generateRule = vm["rule"].as<std::string>();
			auto rule = this->generator->generate(generateRule);
			this->ip->setSpdRule(rule);
		} catch (std::invalid_argument& e) {
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}

		// プロパティ
		if (vm.count("property")) {
			auto properties = vm["property"].as<std::vector<std::string>>();
			for (std::string property : properties) {
				auto propertyPair = parsePropertyOptionValue(property);

				this->ip->getProperties()->push_back(propertyPair);
			}
		}


	} catch (const boost::program_options::multiple_occurrences& e) {
		std::cerr << e.what() << " from option: " << e.get_option_name() << std::endl;
		throw std::exception();
	} catch (std::exception &e) {
		std::cout << e.what() << "\n";
		throw std::exception();
	}
}

std::pair<std::string, std::string> InitParser::parsePropertyOptionValue(
		const std::string& propertyOptValue) const {

	auto colonPos = propertyOptValue.find(':');
	if (colonPos == std::string::npos) {
		throw std::invalid_argument("Please set an initial value for property " +
				propertyOptValue + ".");
	}

	std::pair<std::string, std::string> result {
		propertyOptValue.substr(0, colonPos),
		propertyOptValue.substr(colonPos + 1)
	};

	return result;
}

/*
 * オプションを取得
 */
const std::shared_ptr<boost::program_options::options_description>& InitParser::getOptions() const {
	return this->options;
}

unsigned long InitParser::measureAvailableMemory() const {

	// バイト単位
	unsigned long availableMemory = 0;

	// kB 単位
	unsigned long mem = 0;
	unsigned long buf = 0;

	std::string token;

	try {
		std::ifstream file("/proc/meminfo");
		while(file >> token) {
			if (token == "MemFree:") {
				file >> mem;
			} else if (token == "Inactive:") {
				file >> buf;
			}
		}
	} catch (std::exception& e) {
		// 例外がでたら、握りつぶす
	}

	availableMemory = (mem + buf) * 1024;
	return availableMemory;
}

} /* namespace parser */
} /* namespace param */
} /* namespace spd */
