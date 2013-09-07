/**
 * OriginalType.hpp
 *
 * @date 2012/10/24
 * @author katsumata
 */

#ifndef ORIGINALTYPE_H_
#define ORIGINALTYPE_H_

#include <memory>
#include <vector>

namespace spd {
namespace core {

class Player;

/**
 * プレイヤの持つ近傍の型
 *
 * 半径ごとに、対応するプレイヤ群のvector を入れる。半径は0から始まるため、一つ目のサイズは半径+1
 */
typedef std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::weak_ptr<Player>>>>> Neighbors;

/**
 * 全てのプレイヤの型
 */
typedef std::vector<std::shared_ptr<Player>> AllPlayer;

} /* namespace core */
} /* namespace spd */

#endif /* ORIGINALTYPE_H_ */
