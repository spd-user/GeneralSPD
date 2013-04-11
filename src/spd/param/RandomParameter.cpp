/**
 * RandomParameter.cpp
 *
 * @date 2012/11/09
 * @author katsumata
 */

#include "RandomParameter.hpp"

namespace spd {
namespace param {

/*
 * デフォルトコンストラクタ
 */
RandomParameter::RandomParameter() : discardNum(0), generatedNum(0) {

	std::random_device rd;
	this->seed = rd();
	this->engine = std::mt19937_64(seed);
}

void RandomParameter::showParameter(std::ostream& out) const {

	out << "seed = " << seed << "\n";
	out << "generated-random-numbers = " << discardNum << "\n";
}
} /* namespace param */
} /* namespace spd */
