/**
 * RuntimeParser.cpp
 *
 * @date 2013/01/04
 * @author katsumata
 */

#include <stdexcept>
#include <cmath>
#include <vector>
#include <fstream>
#include "../../core/Action.hpp"
#include "Option.hpp"
#include "RuntimeParser.hpp"
#include "../RuntimeParameter.hpp"

namespace po = boost::program_options;

namespace spd {
namespace param {
namespace parser {

/*
 * コンストラクタ
 */
RuntimeParser::RuntimeParser(const std::shared_ptr<RuntimeParameter>& runtimeParam) : rp(runtimeParam){


	this->options = std::make_shared<po::options_description>("Changeable value in the simulation",
			LINE_LENGTH, MIN_DESCRIPTION_LENGTH);

	options->add_options()
		("strategy-update-cycle,c", po::value<int>()->default_value(rp->getStrategyUpdateCycle()),
				"Player updates own strategy after every interval steps.")
		("self-interaction,i", "Do a self-interaction.")
		("payoff-R,R",
				po::value<double>()->default_value(rp->getPayoff(Action::ACTION_C, Action::ACTION_C)),
				"Reward for mutual cooperation.")
		("payoff-S,S",
				po::value<double>()->default_value(rp->getPayoff(Action::ACTION_C, Action::ACTION_D)),
				"Sucker's payoff.")
		("payoff-T,T",
				po::value<double>()->default_value(rp->getPayoff(Action::ACTION_D, Action::ACTION_C)),
				"Temptation to defect.")
		("payoff-P,P",
				po::value<double>()->default_value(rp->getPayoff(Action::ACTION_D, Action::ACTION_D)),
				"Punishment for mutual defection.");

}

/*
 * オプションの解析を行う
 * @param argc コマンドライン引数
 * @param argv コマンドライン引数値
 * @param allOptions すべてのオプション
 */
void RuntimeParser::parse(
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

		if (vm.count("self-interaction")) {
			this->rp->setSelfInteraction(true);
		}

		this->rp->setStrategyUpdateCycle(std::abs(vm["strategy-update-cycle"].as<int>()));

		this->rp->setPayoffR(vm["payoff-R"].as<double>());
		this->rp->setPayoffS(vm["payoff-S"].as<double>());
		this->rp->setPayoffT(vm["payoff-T"].as<double>());
		this->rp->setPayoffP(vm["payoff-P"].as<double>());

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
 * @return オプション説明
 */
const std::shared_ptr<boost::program_options::options_description>& RuntimeParser::getOptions() const {
	return this->options;
}


} /* namespace parser */
} /* namespace param */
} /* namespace spd */
