/**
 * Topology.hpp
 *
 * @date 2012/09/08
 * @author katsumata
 */

#ifndef TOPOLOGY_H_
#define TOPOLOGY_H_

#include "../IToString.hpp"
#include "../core/OriginalType.hpp"
#include "../core/NeighborhoodType.hpp"

namespace spd {
namespace core {
class Player;
class Space;
}
namespace param {
class Parameter;
}
namespace output {
class OutputVisitor;
}
namespace topology {

/**
 * 空間構造を表すクラス
 */
class Topology : public spd::IToString {
public:

	/**
	 * デストラクタ
	 */
	virtual ~Topology();

	/**
	 * すべてのプレイヤに近傍を設定する
	 *
	 * メモリ容量が足りない場合は設定されない。
	 * @note 半径1の近傍が含まれている場合、接続近傍が必要が設定されている必要がある。
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void setNeighbors(
			const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

	/**
	 * すべてのプレイヤの接続を作成する。
	 *
	 * メモリ容量が足りない場合、プログラムを終了する。
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	virtual void connectPlayers(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param) = 0;

	/**
	 * 指定したプレイヤの、指定近傍半径のプレイヤを取得する
	 * @param[in] players すべてのプレイヤ
	 * @param[in] target 対象プレイヤの空間位置
	 * @param[in] radius 近傍半径
	 */
	virtual spd::core::Neighbors getNeighbors(
			const spd::core::AllPlayer& players,
			int target,
			int radius) = 0;

	/**
	 * 接続と近傍の再設定
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	virtual void reSetting(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param) = 0;

	/**
	 * 空間において必須の戦略の長さを求める
	 * @param actionRadius 行動更新での近傍の長さ
	 * @return 最低限必要な戦略長
	 * @retval 0 最低限がない場合
	 */
	virtual int minStrategyLength(int actionRadius) = 0;

	/**
	 * 空間において最大の戦略の長さを求める
	 * @param actionRadius 行動更新での近傍の長さ
	 * @return 最大の戦略長
	 * @retval 0 できるかぎり使う場合
	 */
	virtual int maxStrategyLength(int actionRadius) = 0;

	/**
	 * 出力方法に構造を知らせる
	 * @param[in] visitor 構造に対する出力の処理方法
	 * @param[in] space 空間
	 */
	virtual void accept(spd::output::OutputVisitor& visitor, spd::core::Space& space) = 0;

	/**
	 * トポロジのプロパティを設定する
	 * @param[in] properties プロパティ
	 */
	virtual void setProp(std::vector<std::string>  properties) = 0;

private:

	/**
	 * プレイヤの近傍をコピーする
	 * @param[in] sourceType コピー元近傍タイプ
	 * @param[in] destType コピー先近傍タイプ
	 * @param[in] players すべてのプレイヤ
	 */
	void copyNeighbors(
			NeighborhoodType sourceType,
			NeighborhoodType destType,
			const spd::core::AllPlayer& players) const;

	/**
	 * プレイヤの接続近傍を、指定した近傍タイプにおける近傍として、コピーする
	 * @param[in] destType コピー先
	 * @param[in] players 全てのプレイヤ
	 */
	void copyConnectors(NeighborhoodType destType, const spd::core::AllPlayer& players) const;

	/**
	 * すべてのプレイヤに指定近傍タイプのプレイヤを設定する
	 * @param[in] neighborType 近傍タイプ
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	virtual void setNeighborsOf(
			NeighborhoodType neighborType,
			const spd::core::AllPlayer& players,
			const spd::param::Parameter& param) = 0;
};

} /* namespace core */
} /* namespace spd */
#endif /* TOPOLOGY_H_ */
