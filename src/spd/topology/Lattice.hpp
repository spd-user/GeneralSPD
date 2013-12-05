/**
 * Lattice.hpp
 *
 * @date 2012/10/05
 * @author katsumata
 */

#ifndef LATTICE_H_
#define LATTICE_H_

#include "Topology.hpp"

namespace spd {
namespace param {
	class InitParameter;
}
namespace topology {

/**
 * 格子構造を表すクラス
 */
class Lattice: public spd::topology::Topology {
public:
	/**
	 * コンストラクタ宣言
	 */
	Lattice();

	/**
	 * デストラクタ
	 */
	virtual ~Lattice();

	/**
	 * 辺の長さを取得
	 * @return 辺の長さ
	 */
	int getSide() const;

	/**
	 * 二次元格子用の出力を行う
	 * @param[in] visitor 構造に対する出力の処理方法
	 * @param[in] space 空間
	 */
	virtual void accept(spd::output::OutputVisitor& visitor, spd::core::Space& space);

protected:
	/**
	 * 空間条件が適合しているか調べる
	 * @note このメソッドは接続近傍を作成する前に実行する必要がある
	 * @param[in] initParam パラメタ
	 * @param[out] errMsg エラーメッセージ
	 * @return 各格子に適切なパラメタかどうか
	 * @retval true 適切に扱える格子の場合
	 * @retval false 適切に扱うことができない格子の場合、エラーメッセージは第2引数から取得
	 */
	virtual bool isCompliant(const std::shared_ptr<spd::param::InitParameter>& initParam, std::string& errMsg) = 0;

	/**
	 * 指定した近傍半径の場合の近傍プレイヤ数を計算する
	 * @param radius 近傍半径
	 * @return 近傍プレイヤ数
	 */
	virtual int calcNeighborsNum(int radius) = 0;

	// 辺の長さ
	int side;
};

} /* namespace topology */
} /* namespace spd */
#endif /* LATTICE_H_ */
