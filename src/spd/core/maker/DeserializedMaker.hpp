/**
 * DeserializedMaker.hpp
 *
 * @date 2013/02/09
 * @author katsumata
 */

#ifndef DESERIALIZEDMAKER_HPP_
#define DESERIALIZEDMAKER_HPP_

#include <memory>
#include <string>
#include "PlayerMaker.hpp"

#include "../Player.hpp"

#include "../msgpack/SerializedPlayer.hpp"
#include "../msgpack/SerializedStrategy.hpp"

namespace spd {
namespace param {
class Parameter;
}
namespace core {
class Space;

/**
 * Msgpack のシリアライズからプレイヤを作成するクラス
 */
class DeserializedMaker : public PlayerMaker {

public:
	/**
	 * シリアライズを指定して初期化
	 * @param[in, out] param パラメタ
	 * @param[in] filename シリアライズ結果
	 */
	DeserializedMaker(spd::param::Parameter& param, std::string filename);

	/**
	 * プレイヤを作成し、接続と近傍設定を行う
	 * @return 全てのプレイヤ
	 */
	AllPlayer makePlayers();

	/**
	 * プレイヤの初期化を行う
	 * @param[in] allPlayer すべてのプレイヤ
	 * @param[in, out] space 盤面
	 * @throw std::runtime_error 戦略の設定が出来ない場合
	 */
	void initPlayer(const AllPlayer& allPlayer, spd::core::Space& space);

	/**
	 * 構造と近傍の再初期化を行う
	 * @param[in, out] allPlayer すべてのプレイヤ
	 */
	void resetTopology(AllPlayer& allPlayer);

private:

	// パラメタ
	spd::param::Parameter& param;

	// ファイル名
	std::string mpacFile;

	// コネクション設定前の使用可能メモリ
	long int memoryForConnection;

	// load connections and set neighbors
	void loadConnectionFromMpac(AllPlayer& allPlayer);

	int findStrategyById(int id, std::vector<serialize::SerializedStrategy>& strategies);

	void restorePlayer(const std::shared_ptr<Player>& player, const serialize::SerializedPlayer& sp);


};

} /* namespace core */
} /* namespace spd */
#endif /* DESERIALIZEDMAKER_HPP_ */
