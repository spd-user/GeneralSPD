/**
 * MembranePropCount.cpp
 *
 * @date 2014/01/27
 * @author katsumata
 */
#include "PropCount.hpp"

#include <stdexcept>

#include "../../../core/Player.hpp"
#include "../../../core/Property.hpp"
#include "../../../core/Strategy.hpp"

namespace spd {
namespace rule {
namespace counting {

/*
 * 特別なプロパティの出力特殊ルール
 * @param[in] allPlayers すべてのプレイヤ
 * @param[in] propPos プレイヤの持つプロパティの中で、対象とするプロパティの位置
 * @return 出力に回す結果
 */
std::map<std::string, int> PropCount::propOutput(
			const spd::core::AllPlayer& allPlayers,
			int propPos) {

	std::map<std::string, int> propertyMap;


	for (auto& player : allPlayers) {

		std::string key = player->getStrategy()->getShortStrategy() + "-" +
				spd::core::converter::actionToChar(player->getAction()) +
				"-" + std::to_string(player->getProperties().at(propPos).getValueAs<int>());

		auto insResult = propertyMap.insert(
				std::pair<std::string, int>(
						key, 1));

		if (!(insResult.second)) {
			propertyMap[key] += 1;
		}
	}
	return propertyMap;

}

} /* namespace counting */
} /* namespace rule */
} /* namespace spd */
