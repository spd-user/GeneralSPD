/**
 * SerializeParam.hpp
 *
 * @date 2013/02/07
 * @author katsumata
 */

#ifndef SERIALIZEPARAM_HPP_
#define SERIALIZEPARAM_HPP_

#include <vector>
#include <string>

#include "Serializer.hpp"
#include "SerializedStrategy.hpp"
#include "../../param/Parameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"
#include "../../param/RuntimeParameter.hpp"
#include "../../param/RandomParameter.hpp"

#include "../../topology/Topology.hpp"

namespace spd {
namespace core {
namespace serialize {

/**
 * パラメタのシリアライズクラス
 *
 * ここでは、パラメタの区分関係なく全てのパラメタをもつ
 */
class SerializedParam {

public:

	/**
	 * デフォルトコンストラクタ
	 */
	SerializedParam() : payoffR (0), payoffS (0), payoffT (0), payoffP (0),
		strategyUpdateCycle (0), selfInteraction(false), generatedRand (0) {}

	/**
	 * パラメタクラスからのコピーコンストラクタ
	 * @param[in] 元となるパラメタ
	 */
	SerializedParam(spd::param::Parameter& param) {

		// 戦略
		auto originalList = param.getStrategyList();
		for (auto original : originalList) {
			strategyList.push_back(SerializedStrategy(original.first));
		}

		// 構造
		topology = param.getNeighborhoodParameter()->getTopology()->toString();

		// シミュレーションで変わるかもしれないパラメタ
		auto& runtimeParam = param.getRuntimeParameter();
		payoffR = runtimeParam->getPayoffR();
		payoffS = runtimeParam->getPayoffS();
		payoffT = runtimeParam->getPayoffT();
		payoffP = runtimeParam->getPayoffP();

		strategyUpdateCycle = runtimeParam->getStrategyUpdateCycle();

		selfInteraction = runtimeParam->isSelfInteraction();

		// 乱数
		generatedRand = param.getRandomParameter()->getGenerated();
	}

	/**
	 * 戦略リストを取得する
	 * @return 戦略リスト
	 */
	const std::vector<SerializedStrategy>& getStrategyList() const {
		return strategyList;
	}

	/**
	 * 空間構造を取得する
	 * @return 空間構造
	 */
	const std::string& getTopology() const {
		return topology;
	}

	/**
	 * payoffRを取得する
	 * @return payoffR
	 */
	double getPayoffR() const {
		return payoffR;
	}

	/**
	 * payoffSを取得する
	 * @return payoffS
	 */
	double getPayoffS() const {
		return payoffS;
	}

	/**
	 * payoffTを取得する
	 * @return payoffT
	 */
	double getPayoffT() const {
		return payoffT;
	}

	/**
	 * payoffPを取得する
	 * @return payoffP
	 */
	double getPayoffP() const {
		return payoffP;
	}

	/**
	 * 自己対戦の有無を取得する
	 * @return 自己対戦の有無
	 */
	bool isSelfInteraction() const {
		return selfInteraction;
	}

	/**
	 * 戦略更新周期を取得する
	 * @return 戦略更新周期
	 */
	int getStrategyUpdateCycle() const {
		return strategyUpdateCycle;
	}

	/**
	 * 乱数生成数を取得する
	 * @return 乱数生成数
	 */
	unsigned long long getGeneratedRand() const {
		return generatedRand;
	}

	/**
	 * シリアライズ
	 */
	MSGPACK_DEFINE(strategyList, topology,
			payoffR, payoffS, payoffT, payoffP,
			strategyUpdateCycle, selfInteraction, generatedRand)

private:

	std::vector<SerializedStrategy> strategyList;

	std::string topology;

	double payoffR;
	double payoffS;
	double payoffT;
	double payoffP;

	int strategyUpdateCycle;

	bool selfInteraction;

	unsigned long long generatedRand;
};

} /* namespace serialize */
} /* namespace core */
} /* namespace spd */
#endif /* SERIALIZEPARAM_HPP_ */
