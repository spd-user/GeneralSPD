/**
 * ConsoleOutput.hpp
 *
 * @date 2012/12/18
 * @author katsumata
 */

#ifndef CONSOLEOUTPUT_H_
#define CONSOLEOUTPUT_H_

#include <memory>
#include "OutputVisitor.hpp"

namespace spd {
namespace core {
class Player;
}
namespace output {

/**
 * 空間状態を標準出力へ表示するクラス
 */
class ConsoleOutput: public OutputVisitor {
public:

	/**
	 * ムーア近傍に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Moore& topology, spd::core::Space& space);

	/**
	 * ノイマン近傍に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Neumann& topology, spd::core::Space& space);

	/**
	 * 六角格子に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Hexagon& topology, spd::core::Space& space);

	/**
	 * ネットワーク構造に即した出力
	 * @note
	 * 標準エラー出力へのエラー出力のみ
	 *
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Network& topology, spd::core::Space& space);

	/**
	 * 初期化
	 * @param[in, out] space 空間
	 * @param[in, out] param パラメタ
	 */
	void init(spd::core::Space& space, spd::param::Parameter& param);

	/**
	 * 出力方法名の出力
	 * @return 出力方法名
	 */
	std::string toString() const {
		return "Console";
	}

private:

	/**
	 * 格子構造の処理
	 * @param[in] lattice 格子空間構造
	 * @param[in] space 空間状態
	 */
	void outputLattice(const spd::topology::Lattice& lattice, spd::core::Space& space);

	/**
	 * 1プレイヤを出力する
	 *
	 * @param[in] player プレイヤ
	 */
	void printPlayer(const std::shared_ptr<spd::core::Player> player);
};

} /* namespace output */
} /* namespace spd */
#endif /* CONSOLEOUTPUT_H_ */
