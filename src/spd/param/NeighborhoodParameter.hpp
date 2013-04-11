/**
 * NeighborhoodParameter.hpp
 *
 * @date 2012/09/20
 * @author katsumata
 */

#ifndef NEIGHBORHOODPARAMETERETER_H_
#define NEIGHBORHOODPARAMETERETER_H_

#include <stdexcept>
#include <memory>
#include "IShowParameter.hpp"
#include "../core/NeighborhoodType.hpp"

namespace spd {
namespace topology {
class Topology;
}

namespace param {

/**
 * 近傍の探索に関わるパラメタを保持するクラス
 */
class NeighborhoodParameter : public IShowParameter {
public:

	/**
	 * デフォルト値で初期化を行う
	 */
	NeighborhoodParameter();

	/**
	 * 指定近傍タイプの近傍半径を取得
	 * @param[in] type 取得する近傍タイプ
	 * @return 指定タイプの近傍半径
	 * @throw std::invalid_argument type が範囲外の場合
	 */
	int getNeiborhoodRadius(NeighborhoodType type) const {

		if (type >= NeighborhoodType::TYPE_NUM) {
			throw std::invalid_argument("No radius for a neighborhood type of " + type);
		}
		return radii[type];
	}

	/**
	 * 指定近傍タイプの近傍半径を設定
	 * @param[in] type 設定する近傍タイプ
	 * @param[in] radius 設定する近傍半径
	 * @throw std::invalid_argument type が範囲外の場合
	 */
	void setNeiborhoodRadius(NeighborhoodType type, int radius) {

		if (type >= NeighborhoodType::TYPE_NUM) {
			throw std::invalid_argument("No radius for a neighborhood type of " + type);
		}
		radii[type] = radius;
	}

	/**
	 * 空間構造を取得
	 * @return 空間構造
	 */
	const std::shared_ptr<spd::topology::Topology>& getTopology() const {
		return topology;
	}

	/**
	 * 空間構造を設定
	 * @param[in] topology 空間構造
	 */
	void setTopology(const std::shared_ptr<spd::topology::Topology>& topology) {
		this->topology = topology;
	}

	/**
	 * パラメタを出力する
	 * @param out 出力先
	 */
	void showParameter(std::ostream& out) const;

private:
	// デフォルト値
	// 各半径
	static const int DEFAULT_RADIUS = 1;

	// パラメタの実態
	// 行動更新半径
	int radii[NeighborhoodType::TYPE_NUM];

	// 近傍取得方法
	std::shared_ptr<spd::topology::Topology> topology;
};

} /* namespace param */
} /* namespace spd */
#endif /* NEIGHBORHOODPARAMETERETER_H_ */
