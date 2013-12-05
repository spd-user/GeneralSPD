/**
 * OutputVisitor.hpp
 *
 * @date 2012/12/19
 * @author katsumata
 */

#ifndef OUTPUTVISITOR_H_
#define OUTPUTVISITOR_H_

#include "Output.hpp"

namespace spd {
namespace core {
class Space;
}
namespace topology {

class Lattice;
class Neumann;
class Hexagon;
class Moore;
class Network;
class Cube;
}

namespace output {

/**
 * 構造によって処理を変更する出力クラス
 */
class OutputVisitor : public Output {
public:

	/**
	 * デストラクタ
	 */
	virtual ~OutputVisitor();

	/**
	 * 構造によって処理を変更する
	 * @param[in] space 空間情報
	 * @return 現在は圧縮が必要なのはないので、second にfalseがあるペア
	 */
	std::pair<std::string, bool> output(spd::core::Space& space);


	/**
	 * 二次元格子に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	virtual void output(const spd::topology::Lattice& topology, spd::core::Space& space) = 0;

	/**
	 * 立方体に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	virtual void output(const spd::topology::Cube& topology, spd::core::Space& space) = 0;

	/**
	 * ネットワーク構造に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	virtual void output(const spd::topology::Network& topology, spd::core::Space& space) = 0;
};

} /* namespace output */
} /* namespace spd */
#endif /* OUTPUTVISITOR_H_ */
