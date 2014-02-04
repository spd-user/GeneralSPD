/**
 * MembranePropCount.hpp
 *
 * @date 2014/01/27
 * @author katsumata
 */
#ifndef PROPCOUNT_HPP_
#define PROPCOUNT_HPP_

#include "../../../core/PropertyCounting.hpp"

namespace spd {
namespace rule {
namespace counting {

/**
 * 膜用のプロパティカウントを表すクラス
 */
class PropCount : public spd::core::PropertyCounting {
public :

	~PropCount(){};

	/**
	 * 膜検知プロパティのカウンティングに燗する特殊ルール
	 * @param[in] allPlayers すべてのプレイヤ
	 * @param[in] propPos プレイヤの持つプロパティの中で、対象とするプロパティの位置
	 * @return 出力に回す結果
	 */
	std::map<std::string, int> propOutput(
			const spd::core::AllPlayer& allPlayers,
			int propPos);
};

} /* namespace counting */
} /* namespace rule */
} /* namespace spd */

#endif /* PROPCOUNT_HPP_ */
