/**
 * PlayerMaker.hpp
 *
 * @date 2013/01/12
 * @author katsumata
 */

#ifndef PLAYERMAKER_H_
#define PLAYERMAKER_H_

#include "../OriginalType.hpp"

namespace spd {
namespace core {

class Space;

/**
 * プレイヤを作成の抽象クラス
 */
class PlayerMaker {
public:

	/**
	 * デストラクタ
	 */
	virtual ~PlayerMaker(){};

	/**
	 * プレイヤを作成し、接続と近傍の設定を行う
	 * @return 全てのプレイヤ
	 */
	virtual AllPlayer makePlayers() = 0;

	/**
	 * プレイヤの初期化を行う
	 * @note
	 * プレイヤの近傍設定については操作しない。操作する場合は、resetTopology()を呼ぶ。
	 * @param[in] players すべてのプレイヤ
	 * @param[in, out] space 盤面
	 */
	virtual void initPlayer(const AllPlayer& players, spd::core::Space& space) = 0;

	/**
	 * 構造と近傍の再初期化を行う
	 * @param[in, out] players すべてのプレイヤ
	 */
	virtual void resetTopology(AllPlayer& players) = 0;

};

} /* namespace core */
} /* namespace spd */
#endif /* PLAYERMAKER_H_ */
