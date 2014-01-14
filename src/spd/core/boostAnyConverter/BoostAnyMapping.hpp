/**
 * BoostAnyMapping.hpp
 *
 * @date 2014/01/14
 * @author katsumata
 */
#ifndef BOOSTANYMAPPING_HPP_
#define BOOSTANYMAPPING_HPP_

#include <typeinfo>
#include <string>

#include <boost/any.hpp>

namespace spd {
namespace core {
namespace converter {

/**
 * boost::any と string の対応を表すクラス
 */
class BoostAnyMapping {
public:

	/**
	 * デストラクタ
	 */
	virtual ~BoostAnyMapping() {};

	/**
	 * typeid 名前要素を取得
	 * @return 名前要素
	 */
	virtual std::string getTypeName() = 0;

	/**
	 * gexfでの型名を取得
	 * @return gexfでの型名
	 */
	virtual std::string getStringName() = 0;

	/**
	 * 文字列を boost::any の値に変換する
	 * @param val string の値
	 * @return 変換結果
	 */
	virtual boost::any stringToAny(std::string val) = 0;

	/**
	 * boost::any を文字列に変換する
	 * @param val boost::any の値
	 * @return 変換結果
	 */
	virtual std::string anyToString(boost::any val) = 0;
};

} /* namespace converter */
} /* namespace core */
} /* namespace spd */

#endif /* BOOSTANYMAPPING_HPP_ */
