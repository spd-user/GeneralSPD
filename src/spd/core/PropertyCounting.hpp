/**
 * PropertyCounting.hpp
 *
 * @date 2014/01/27
 * @author katsumata
 */
#ifndef PROPERTYCOUNTING_HPP_
#define PROPERTYCOUNTING_HPP_

#include <map>
#include <string>

#include "OriginalType.hpp"

namespace spd {
namespace core {

/**
 * プロパティの出力に数え上げを提供する
 */
class PropertyCounting {
public:

	/**
	 * デストラクタ
	 */
	virtual ~PropertyCounting(){};

	/**
	 * 特別なプロパティの出力特殊ルール
	 * @param[in] allPlayers すべてのプレイヤ
	 * @param[in] propPos プレイヤの持つプロパティの中で、対象とするプロパティの位置
	 * @return 出力に回す結果
	 */
	virtual std::map<std::string, int> propOutput(
			const AllPlayer& allPlayers,
			int propPos) = 0;

};

}
}


#endif /* PROPERTYCOUNTING_HPP_ */
