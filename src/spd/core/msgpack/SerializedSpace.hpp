/**
 * SerializedSpace.hpp
 *
 * @date 2013/02/08
 * @author katsumata
 */

#ifndef SERIALIZEDSPACE_HPP_
#define SERIALIZEDSPACE_HPP_

#include <map>
#include <vector>

#include "Serializer.hpp"

#include "../Space.hpp"
#include "../Player.hpp"
#include "../Strategy.hpp"
#include "../Converter.hpp"

#include "SerializedPlayer.hpp"
#include "SerializedParam.hpp"


namespace spd {
namespace core {
namespace serialize {

/**
 * 空間用のシリアライズクラス
 */
class SerializedSpace {

public:

	/**
	 * デフォルトコンストラクタ
	 */
	SerializedSpace() : step (0) {};

	/**
	 * 空間クラスからのコピーコンストラクタ
	 * @param[in] space 元となる空間
	 */
	SerializedSpace (Space& space) {

		this->step = space.getStep();

		param = SerializedParam(space.getParameter());

		auto& allPlayer = space.getPlayers();
		int playerNum = allPlayer.size();
		links.reserve(playerNum);

		for (int i = 0; i < playerNum; ++i) {
			// 情報を読み出す
			SerializedPlayer sp (allPlayer[i]);
			int id = sp.getId();

			std::vector<int> pv = {id};

			auto result = playersMap.insert(std::pair<SerializedPlayer, std::vector<int>>(sp, pv));
			if (!(result.second)) {
				playersMap[sp].push_back(id);
			}

			auto& link = allPlayer[i]->getLinkedPlayers();
			std::vector<int> playerLink;
			// 空にならないよう自分をいれておく
			playerLink.push_back(id);
			for (auto lp : *(link)) {
				int opponentId = lp.lock()->getId();
				// 相手が高い場合のみ保存
				if (opponentId > id) {
					playerLink.push_back(lp.lock()->getId());
				}
			}
			links.push_back(std::move(playerLink));
		}
	}

	/**
	 * ステップ数を取得する
	 * @return ステップ数
	 */
	int getStep() const {
		return step;
	}

	/**
	 * 情報が同じプレイヤの一覧表を取得する
	 * @return 基礎情報が同じプレイヤの一覧表
	 */
	const std::map<SerializedPlayer, std::vector<int>>& getPlayersMap() const {
		return playersMap;
	}

	/**
	 * プレイヤの接続の一覧を取得する
	 * @return プレイヤの接続の一覧
	 */
	const std::vector<std::vector<int>>& getLinks() const {
		return links;
	}

	/**
	 * パラメタを取得する
	 * @return パラメタ
	 */
	const SerializedParam& getParam() const {
		return param;
	}

	/**
	 * シリアライズ
	 */
	MSGPACK_DEFINE(step, param, playersMap, links);

private:

	// step
	int step;

	// parameter
	SerializedParam param;

	// 情報が同じプレイヤのリスト
	std::map<SerializedPlayer, std::vector<int>> playersMap;

	// プレイヤのリンクリスト
	std::vector<std::vector<int>> links;
};

} /* namespace serialize */
} /* namespace core */
} /* namespace spd */
#endif /* SERIALIZEDSPACE_HPP_ */

