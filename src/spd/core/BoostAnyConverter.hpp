/**
 * BoostAnyConverter.hpp
 *
 * @date 2013/02/09
 * @author katsumata
 */

#ifndef BOOSTANYCONVERTER_HPP_
#define BOOSTANYCONVERTER_HPP_


#include <memory>
#include <typeinfo>
#include <tuple>
#include <string>
#include <functional>
#include <vector>
#include <stdexcept>

#include <boost/any.hpp>

#include "boostAnyConverter/AllAnyConverter.hpp"

namespace spd {
namespace core {
namespace converter {

/**
 * 組み合わせ一覧
 */
const std::vector<std::shared_ptr<spd::core::converter::BoostAnyMapping>> CONVERTER_MAP =
{
		std::shared_ptr<spd::core::converter::BoostAnyMapping>(new spd::core::converter::IntAny()),
		std::shared_ptr<spd::core::converter::BoostAnyMapping>(new spd::core::converter::DoubleAny()),
		std::shared_ptr<spd::core::converter::BoostAnyMapping>(new spd::core::converter::BoolAny()),
		std::shared_ptr<spd::core::converter::BoostAnyMapping>(new spd::core::converter::StringAny())
};

/**
 * 型に応じた文字列の型名を取得する
 *　@param[in] type 型情報
 *　@return 独自の型名
 *　@retval "object" 該当しない場合
 */
inline std::string getTypeName(const std::type_info& type) {
	std::string typeName (type.name());

	for (auto& anyConverter : CONVERTER_MAP) {
		if (anyConverter->getTypeName() == typeName) {
			return anyConverter->getStringName();
		}
	}
	return "object";
};




/**
 * 独自の型名から文字列を対応する boost::any に変換する
 * @param[in] typeName 独自の型名
 * @param[in] val 値
 * @return boost::any にした結果
 * @throw invalid_argument 独自型名から変換できない場合
 */
inline boost::any stringToBoostAny(const std::string& typeName, const std::string& val) {

	for (auto& anyConverter : CONVERTER_MAP) {
		if (anyConverter->getStringName() == typeName) {
			return anyConverter->stringToAny(val);
		}
	}

	throw std::invalid_argument("Could not convert to a boost::any.");
}


/***
 * boost の値を文字列に変換する
 * @param[in] val boost::any の値
 */
inline std::string boostAnyToString(const boost::any& val) {
	std::string typeName (val.type().name());

	for (auto& anyConverter : CONVERTER_MAP) {
		if (anyConverter->getStringName() == typeName) {
			return anyConverter->anyToString(val);
		}
	}

	throw std::invalid_argument("Could not convert and express to a string.");
}

}
}
}

#endif /* BOOSTANYCONVERTER_HPP_ */
