/**
 * NeighborhoodType.hpp
 *
 * @date 2012/10/12
 * @author katsumata
 */

#ifndef NEIGHBORHOODTYPE_H_
#define NEIGHBORHOODTYPE_H_

/**
 * プレイヤの持つ近傍の種類のタイプを表す
 */
enum NeighborhoodType {
	ACTION, /**< 行動更新用近傍 */
	GAME, /**< 対戦用近傍 */
	STRATEGY, /**< 戦略更新用近傍 */
	TYPE_NUM, /**< タイプの数 */
};

#endif /* NEIGHBORHOODTYPE_H_ */
