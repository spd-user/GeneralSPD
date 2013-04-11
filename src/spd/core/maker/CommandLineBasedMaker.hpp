/**
 * CommandLineBasedMaker.hpp
 *
 * @date 2013/01/12
 * @author katsumata
 */

#ifndef COMMANDLINEBASEDMAKER_H_
#define COMMANDLINEBASEDMAKER_H_

#include "PlayerMaker.hpp"

namespace spd {
namespace param {
class Parameter;
}
namespace core {
class Space;

/**
 * コマンドラインからプレイヤを作成するクラス
 */
class CommandLineBasedMaker : public PlayerMaker {

public:

	/**
	 * 初期化で使用するパラメタを指定して初期化
	 * @param[in, out] param パラメタ
	 */
	CommandLineBasedMaker(spd::param::Parameter& param);

	/**
	 * プレイヤの接続と近傍設定を行う
	 * @return 全てのプレイヤ
	 */
	AllPlayer makePlayers();

	/**
	 * プレイヤの初期化を行う
	 * @param[in, out] players すべてのプレイヤ
	 * @param[in, out] space 盤面
	 * @throw std::runtime_error 戦略の設定が出来ない場合
	 */
	void initPlayer(const AllPlayer& players, spd::core::Space& space);

	/**
	 * 構造と近傍の再初期化を行う
	 * @param[in, out] players すべてのプレイヤ
	 */
	void resetTopology(AllPlayer& players);

private:

	/*
	 * 初期化で使用するパラメタ
	 */
	spd::param::Parameter& parameter;

	/**
	 * 戦略番号を取得する
	 * @return 戦略番号
	 * @retval 戦略が見つからない場合 -1
	 */
	int getStrategyNumber();
};

} /* namespace core */
} /* namespace spd */
#endif /* COMMANDLINEBASEDMAKER_H_ */
