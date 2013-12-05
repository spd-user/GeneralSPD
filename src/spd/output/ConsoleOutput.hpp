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
	 * 二次元格子に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Lattice& topology, spd::core::Space& space);

	/**
	 * 立体格子に即した出力
	 * @param[in] topology 空間構造
	 * @param[in] space 空間状態
	 */
	void output(const spd::topology::Cube& topology, spd::core::Space& space);

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
	 * 1プレイヤを出力する
	 *
	 * @param[in] player プレイヤ
	 */
	void printPlayer(const std::shared_ptr<spd::core::Player> player);
};

} /* namespace output */
} /* namespace spd */
#endif /* CONSOLEOUTPUT_H_ */
