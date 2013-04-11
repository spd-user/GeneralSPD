/**
 * NeighborhoodParameter.cpp
 *
 * @date 2012/09/20
 * @author katsumata
 */

#include "NeighborhoodParameter.hpp"

#include "../topology/Moore.hpp"

namespace spd {
namespace param {

/*
 * デフォルトコンストラクタ
 */
NeighborhoodParameter::NeighborhoodParameter() {

	for (int i = 0; i < NeighborhoodType::TYPE_NUM; ++i) {
		this->radii[i] = DEFAULT_RADIUS;
	}

	// デフォルトはムーア近傍
	topology = std::make_shared<spd::topology::Moore>();
}

/*
 * 表示
 */
void NeighborhoodParameter::showParameter(std::ostream& out) const {

	// 行動更新半径
	out << "action-radius = " << radii[NeighborhoodType::ACTION] << "\n";
	out << "game-radius = " << radii[NeighborhoodType::GAME] << "\n";
	out << "strategy-update-radius = " << radii[NeighborhoodType::STRATEGY] << "\n";

	// 近傍取得方法
	out << "topology = " << topology->toString() << "\n";
}

} /* namespace param */
} /* namespace spd */
