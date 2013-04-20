/**
 * PropertyTest.cpp
 *
 * @date 2013/04/20
 * @author katsumata
 */

#include "PropertyTest.hpp"
#include <string>
#include <stdexcept>

#include "../../core/Player.hpp"
#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"


namespace spd {
namespace rule {

void PropertyTest::initialize(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param) {

	bool optExist = false;
	std::string optVal = "";

	auto propertyOpts = *(param.getInitialParameter()->getProperties().get());
	for (auto& val : propertyOpts) {
		if (val.first == "propTest") {
			optExist = true;
			optVal = val.second;
		}
	}

	if (!optExist) {
		throw std::runtime_error("For test prop, input prop opt [propTest]");
	}

	try {
		auto& prop = player->getProperty("testProp");
		int val = 0;
		prop.setValue(val);
	} catch (std::invalid_argument& e) {

		spd::core::Property p ("testProp", 0, spd::core::Property::OutputType::CLASSIFIABLE);
		player->addProperty(p);
	}

}

void PropertyTest::runRule(
	const std::shared_ptr<Player>& player,
	const AllPlayer& allPlayers,
	const spd::param::Parameter& param,
	int step) {

	int val = 0;

	auto& prop = player->getProperty("testProp");
	if (player->getAction() == Action::ACTION_C) {
		val = 1;
	}

	prop.setValue(val);


}


} /* namespace rule */
} /* namespace spd */
