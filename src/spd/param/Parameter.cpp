/**
 * Parameter.cpp
 *
 * @date 2012/09/05
 * @author katsumata
 */

#include "Parameter.hpp"

#include <vector>
#include <memory>

#include "InitParameter.hpp"
#include "OutputParameter.hpp"
#include "RuntimeParameter.hpp"
#include "NeighborhoodParameter.hpp"
#include "RandomParameter.hpp"

#include "../core/Strategy.hpp"
#include "../core/Action.hpp"

namespace spd {
namespace param {

/*
 * デフォルト値で初期化
 */
Parameter::Parameter() : core(1) {
	std::vector<std::pair<std::shared_ptr<core::Strategy>, int>> strategyList;
	this->strategyList = strategyList;

	std::vector<std::pair<std::shared_ptr<core::Strategy>, int>> s_strategyList;
	this->s_strategyList = s_strategyList;

	this->initParam = std::make_shared<InitParameter>();
	this->neighborParam = std::make_shared<NeighborhoodParameter>();
	this->outputParam = std::make_shared<OutputParameter>();
	this->runtimeParam = std::make_shared<RuntimeParameter>();
	this->randomParam = std::make_shared<RandomParameter>();
}

void Parameter::store() {

	s_strategyList.clear();
	for (auto strategyPair : strategyList) {
		s_strategyList.push_back(strategyPair);
	}

	runtimeParam->store();
}

void Parameter::restore() {

	strategyList.clear();
	for (auto strategyPair : s_strategyList) {
		strategyList.push_back(strategyPair);
	}

	runtimeParam->restore();
}

/*
 * getter
 */
const std::vector<std::pair<std::shared_ptr<core::Strategy>, int>>& Parameter::getStrategyList() const {
	return strategyList;
}

void Parameter::addStrategy(std::pair<std::shared_ptr<core::Strategy>, int> strategyPair) {

	for (int i = 0, size = this->strategyList.size(); i < size; ++i){
		if (strategyList.at(i).first->equals(*(strategyPair.first))) {

			strategyList.at(i).second = strategyList.at(i).second + strategyPair.second;
			return;
		}
	}
	strategyPair.first->setId(strategyList.size());
	this->strategyList.push_back(strategyPair);
}

/*
 * すべてのパラメタを出力する
 */
void Parameter::showParameter(std::ostream& out) const {

	out << "#---------------------------------#\n";
	if (strategyList.size() != 0) {
		for (auto strategy : strategyList) {
			out << "strategy = " << strategy.first->getShortStrategy() << ":" << strategy.second << "\n";
		}
	} else {
		out << "# Nothing strategy\n";
	}

	// それぞれのパラメタの表示
	initParam->showParameter(out);
	runtimeParam->showParameter(out);
	neighborParam->showParameter(out);
	outputParam->showParameter(out);
	randomParam->showParameter(out);

	out << "core = " << core <<
			"\n#---------------------------------#\n";

}

} /* namespace core */
} /* namespace spd */
