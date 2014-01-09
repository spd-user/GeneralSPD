/**
 * RepairRule.hpp
 *
 * @date 2014/01/09
 * @author katsumata
 */
#ifndef REPAIRRULE_HPP_
#define REPAIRRULE_HPP_

#include "../Rule.hpp"

namespace spd {
namespace rule {

/**
 * 自己修復のルールを表すクラス
 */
class RepairRule : public spd::rule::Rule {
public:

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
		return "repair";
	}
};

} /* namespace rule */
} /* namespace spd */

#endif /* REPAIRRULE_HPP_ */
