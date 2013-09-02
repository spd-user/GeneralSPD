/**
 * SerializePlayer.hpp
 *
 * @date 2013/02/06
 * @author katsumata
 */

#ifndef SERIALIZEPLAYER_H_
#define SERIALIZEPLAYER_H_

#include <memory>

#include "Serializer.hpp"

#include "../Converter.hpp"
#include "../Player.hpp"
#include "../Strategy.hpp"
#include "SerializedProperty.hpp"

namespace spd {
namespace core {
namespace serialize {

/**
 * プレイヤ用のシリアライズクラス
 */
class SerializedPlayer {

public :

	/**
	 * シリアライズ用のコンストラクタ
	 */
	SerializedPlayer() : id(-1), preAction(0) , action(0),
		preScore(0), score(0),
		preStrategyId(0), strategyId(0) {}

	/**
	 * プレイヤクラスからのコピーコンストラクタ
	 * @param[in] player 元となるプレイヤ
	 */
	SerializedPlayer(const std::shared_ptr<Player>& player) {
		id = player->getId();

		preAction = static_cast<int>(player->getPreAction());
		action = static_cast<int>(player->getAction());

		preScore = player->getPreScore();
		score = player->getScore();

		preStrategyId = player->getPreStrategy()->getId();
		strategyId = player->getStrategy()->getId();

		auto& originalProp = player->getProperties();
		for (auto prop : originalProp) {
			propertyies.push_back(SerializedProperty(prop));
		}
	}

	/**
	 * プレイヤIDを取得する
	 * @return プレイヤID
	 */
	int getId() const {
		return id;
	}

	/**
	 * 行動の番号を取得する
	 * @return 行動の番号
	 */
	int getAction() const {
		return action;
	}

	/**
	 * 前の行動の番号を取得する
	 * @return 前の行動の番号
	 */
	int getPreAction() const {
		return preAction;
	}

	/**
	 * 前のスコアを取得する
	 * @return 前のスコア
	 */
	double getPreScore() const {
		return preScore;
	}

	/**
	 * スコアを取得する
	 * @return スコア
	 */
	double getScore() const {
		return score;
	}

	/**
	 * 前の戦略IDを取得する
	 * @return 前の戦略ID
	 */
	int getPreStrategyId() const {
		return preStrategyId;
	}

	/**
	 * 戦略IDを取得する
	 * @return 戦略ID
	 */
	int getStrategyId() const {
		return strategyId;
	}

	/**
	 * プロパティを取得する
	 * @return プロパティ
	 */
	const std::vector<SerializedProperty>& getProperties() const {
		return propertyies;
	}

	/**
	 * キーのソートは現在のscore 順とする
	 * @return 比較結果
	 */
	bool operator<(const SerializedPlayer& sp) const {
		return score < sp.getScore();
	}

	/**
	 * シリアライズ
	 */
	MSGPACK_DEFINE(id, preAction, action, preScore, score, preStrategyId, strategyId, propertyies);

private :

	int id;

	int preAction;
	int action;

	double preScore;
	double score;

	int preStrategyId;
	int strategyId;

	std::vector<SerializedProperty> propertyies;
};

} /* namespace serialize */
} /* namespace core */
} /* namespace spd */

#endif /* SERIALIZEPLAYER_H_ */
