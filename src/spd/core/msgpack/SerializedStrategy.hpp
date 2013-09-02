/**
 * SerializeStrategy.hpp
 *
 * @date 2013/02/06
 * @author katsumata
 */

#ifndef SERIALIZESTRATEGY_H_
#define SERIALIZESTRATEGY_H_

#include <string>
#include <memory>

#include "Serializer.hpp"
#include "../Strategy.hpp"

namespace spd {
namespace core {
namespace serialize {

/**
 * 戦略用のシリアライズクラス
 */
class SerializedStrategy {
public:

	/**
	 * デフォルトコンストラクタ
	 */
	SerializedStrategy() : id(0) {};

	/**
	 * 戦略クラスからのコピーコンストラクタ
	 * @param[in] strategy 元となる戦略
	 */
	SerializedStrategy(const std::shared_ptr<Strategy>& strategy) {
		id = strategy->getId();

		shortStrategy = strategy->getShortStrategy();
	}

	/**
	 * id を取得する
	 * @return id
	 */
	int getId() const {
		return id;
	}

	/**
	 * 省略戦略を取得する
	 * @return 省略戦略
	 */
	std::string getShortStrategy() const {
		return this->shortStrategy;
	}

	/**
	 * シリアライズ
	 */
	MSGPACK_DEFINE(id, shortStrategy);

private:

	// id
	int id;

	// 略記戦略
	std::string shortStrategy;
};

} /* namespace serialize */
} /* namespace core */
} /* namespace spd */
#endif /* SERIALIZESTRATEGY_H_ */
