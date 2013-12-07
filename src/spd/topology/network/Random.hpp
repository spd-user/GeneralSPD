/**
 * Random.hpp
 *
 * @date 2013/09/05
 * @author katsumata
 */

#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <memory>
#include <string>
#include "Network.hpp"

namespace spd {
namespace topology {

/**
 * ランダムグラフを表すクラス
 */
class Random: public spd::topology::Network {

public :

	/**
	 * デフォルトの接続確率でクラス生成
	 */
	Random();

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
	 * トポロジのプロパティ(接続確率)を設定する
	 *
	 * もし複数のプロパティが渡されても、一つ目しか使用しない<br>
	 * また、無い場合は変更しないまま
	 * @param[in] properties プロパティ
	 */
	void setProp(std::vector<std::string> properties);

	/**
	 * 空間構図構造名の出力
	 * @return 空間構図構造名(Random)
	 */
	std::string toString() const {
		std::string result;

		result = "Random:" + std::to_string(this->connectionProbability);
		return result;
	}

private :

	/**
	 * 追加生成か減少生成かの閾値
	 */
	static constexpr double DIVIDE_POINT = 0.6;

	/**
	 * デフォルトの接続確率
	 */
	static constexpr double DEFAULT_CONNECTION_PROBABILITY = 0.01;

	/**
	 * ノードの追加型接続生成
	 *
	 * メモリ容量が足りない場合、プログラムを終了する。
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void incrementCreate(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);


	/**
	 * ノードの減少生成型接続生成
	 *
	 * メモリ容量が足りない場合、追加型生成に切り替える
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void decrementCreate(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

	/**
	 * 接続確率
	 */
	double connectionProbability;

	/**
	 * 接続に使用したメモリ
	 */
	unsigned long int usedMemory;

};

} /* namespace topology */
} /* namespace spd */
#endif /* RANDOM_HPP_ */
