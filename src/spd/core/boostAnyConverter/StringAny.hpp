/**
 * StringAny.hpp
 *
 * @date 2014/01/14
 * @author katsumata
 */
#ifndef STRINGANY_HPP_
#define STRINGANY_HPP_

#include "BoostAnyMapping.hpp"

namespace spd {
namespace core {
namespace converter {

/**
 * String と boost::any の変換を表すクラス
 */
class StringAny : public BoostAnyMapping {

public:

	/**
	 * typeid 名前要素を取得
	 * @return 名前要素
	 */
	std::string getTypeName() {
		return typeid(std::string).name();
	}

	/**
	 * gexfでの型名を取得
	 * @return gexfでの型名
	 */
	std::string getStringName() {
		return "string";
	}

	/**
	 * 文字列を boost::any の値に変換する
	 * @param val string の値
	 * @return 変換結果
	 */
	boost::any stringToAny(std::string val) {
		boost::any result = val;
		return result;
	}

	/**
	 * boost::any を文字列に変換する
	 * @param val boost::any の値
	 * @return 変換結果
	 */
	std::string anyToString(boost::any val) {
		std::string result = boost::any_cast<std::string>(val);
		return result;
	}
};

} /* namespace converter */
} /* namespace core */
} /* namespace spd */


#endif /* STRINGANY_HPP_ */
