/**
 * PropertyTest.hpp
 *
 * @date 2013/04/20
 * @author katsumata
 */

#ifndef PROPERTYTEST_HPP_
#define PROPERTYTEST_HPP_

#include "../Rule.hpp"

namespace spd {
namespace rule {

/**
 * ぷろぱてぃのてすとを表すクラス
 */
class PropertyTest: public spd::rule::Rule {


	void initialize(
			const std::shared_ptr<Player>& player,
			const AllPlayer& allPlayers,
			const spd::param::Parameter& param);


	void runRule(
		const std::shared_ptr<Player>& player,
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step);


	std::string toString() const {
		return "TestProp";
	}
};

} /* namespace rule */
} /* namespace spd */
#endif /* PROPERTYTEST_HPP_ */
