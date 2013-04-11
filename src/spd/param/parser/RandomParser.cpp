/**
 * RandomParser.cpp
 *
 * @date 2013/01/04
 * @author katsumata
 */

#include <stdexcept>
#include <fstream>
#include "Option.hpp"
#include "RandomParser.hpp"
#include "../RandomParameter.hpp"

namespace po = boost::program_options;

namespace spd {
namespace param {
namespace parser {

/*
 * コンストラクタ
 */
RandomParser::RandomParser(const std::shared_ptr<RandomParameter>& randomParam) : rp(randomParam) {

	// オプションの作成
	this->options = std::make_shared<po::options_description>("Random value",
			LINE_LENGTH, MIN_DESCRIPTION_LENGTH);

	// オプションの設定
	options->add_options()
		("generated-random-numbers", 	po::value<unsigned long long>()->default_value(rp->getDiscardNum()),
				"Advances the random number generator's internal status, before starting the simulation.")
		("seed", 							po::value<unsigned int>()->default_value(rp->getSeed()),
				"Random number generator's seed.");

}

/*
 * オプションの解析を行う
 * @param argc コマンドライン引数
 * @param argv コマンドライン引数値
 * @param allOptions すべてのオプション
 */
void RandomParser::parse(
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

		// 乱数シード
		this->rp->setSeed(vm["seed"].as<unsigned int>());

		this->rp->setDiscardNum(vm["generated-random-numbers"].as<unsigned long long>());

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
const std::shared_ptr<boost::program_options::options_description>& RandomParser::getOptions() const {
	return this->options;

}


} /* namespace parser */
} /* namespace param */
} /* namespace spd */
