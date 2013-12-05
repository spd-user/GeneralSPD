/**
 * Hexagon.hpp
 *
 * @date 2012/12/19
 * @author katsumata
 */

#ifndef HEXAGON_H_
#define HEXAGON_H_

#include <memory>
#include "Lattice.hpp"

namespace spd {
namespace topology {

/**
 * 六角格子の空間構造を表すクラス
 */
class Hexagon: public spd::topology::Lattice {
public:

	/**
	 * コンストラクタ
	 */
	Hexagon();

	/**
	 * すべてのプレイヤの接続を作成する。
	 *
	 * メモリ容量が足りない場合、プログラムを終了する。
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void connectPlayers(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

	/**
	 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
	 * @param[in] players すべてのプレイヤ
	 * @param[in] target 対象プレイヤの空間位置
	 * @param[in] radius 近傍半径
	 * @throw std::invalid_argument 近傍半径が0未満の場合
	 */
	spd::core::Neighbors getNeighbors(
			const spd::core::AllPlayer& players,
			int target,
			int radius);

	/**
	 * 接続と近傍の再設定
	 *
	 * @note なにもしない
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void reSetting(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

	/**
	 * 空間において必須の戦略の長さを求める
	 * @param[in] actionRadius 行動更新での近傍距離
	 * @return 最低限必要な戦略長
	 * @retval 近傍数+1
	 */
	int minStrategyLength(int actionRadius);

	/**
	 * 空間において最大の戦略の長さを求める
	 * @param[in] actionRadius 行動更新での近傍の長さ
	 * @return 最大の戦略長
	 * @retval 近傍数+1
	 */
	int maxStrategyLength(int actionRadius);

	/**
	 * トポロジのプロパティを設定する
	 * @note プロパティはないのでなにもしない
	 * @param[in] properties プロパティ
	 */
	void setProp(std::vector<std::string> properties) {
		return;
	};

	/**
	 * 空間構図構造名の出力
	 * @return 空間構図構造名(Moore)
	 */
	std::string toString() const;

protected:

	/**
	 * 空間条件が適合しているか調べる
	 * @param[in] initParam パラメタ
	 * @param[out] errMsg エラーメッセージ
	 * @return 各格子に適切なパラメタかどうか
	 * @retval true 適切に扱える格子の場合
	 * @retval false 適切に扱うことができない格子の場合、エラーメッセージは第2引数から取得
	 */
	bool isCompliant(const std::shared_ptr<spd::param::InitParameter>& initParam, std::string& errMsg);

	/**
	 * 指定した近傍半径の自身を含めたプレイヤ数を計算する
	 * @param radius 近傍半径
	 * @return 自身を含めた近傍プレイヤ数
	 */
	int calcNeighborsNum(int radius);

private:

	// 隣接プレイヤ数
	static const int ADJACENCE = 6;

	/**
	 * すべてのプレイヤに指定近傍タイプのプレイヤを設定する
	 *
	 * @note 近傍半径が0未満の場合やメモリが足りないプレイヤの近傍は、nullptr で埋める
	 * @param[in] neighborType 近傍タイプ
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void setNeighborsOf(
			NeighborhoodType neighborType,
			const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

};

} /* namespace topology */
} /* namespace spd */
#endif /* HEXAGON_H_ */
