/**
 * BoostAnyConverter.hpp
 *
 * @date 2013/02/09
 * @author katsumata
 */

#ifndef BOOSTANYCONVERTER_HPP_
#define BOOSTANYCONVERTER_HPP_

#include <typeinfo>
#include <tuple>
#include <string>
#include <functional>
#include <vector>
#include <stdexcept>

#include <boost/any.hpp>

namespace spd {
namespace core {
namespace converter {

/**
 * 型名、型名の文字列と string と boost::any の相互変換の組み合わせ
 *
 * @par
 * 1つ目は、typeidの name 関数要素。
 * 2つ目は、それを表す文字列　gexf などで使う独自の型名。
 * 3つ目は、文字列を boost::any にするための変換式。
 * 4つ目は、boost::any を文字列にする変換式。
 */
typedef std::tuple<std::string, std::string, std::function<boost::any(std::string)>,
		std::function<std::string(boost::any)>> AnyTypeTuple;

/**
 * 組み合わせ一覧
 */
const std::vector<AnyTypeTuple> CONVERTER_MAP =
{
		std::make_tuple(typeid(int).name(), 		"integer",
				[](std::string val) { return std::stoi(val);},
				[](boost::any val) { return std::to_string( boost::any_cast<int>(val)); } ),

		std::make_tuple(typeid(double).name(), 	"double",
				[](std::string val) { return std::stod(val); },
				[](boost::any val) { return std::to_string(boost::any_cast<double>(val)); }),

		std::make_tuple(typeid(bool).name(), 		"boolean",
				[](std::string val) { return (val == "true")? true : false; },
				[](boost::any val) { return boost::any_cast<bool>(val) ? "true" : "false" ;}),

		std::make_tuple(typeid(std::string).name(), "string",
				[](std::string val) { return val; },
				[](boost::any val) { return boost::any_cast<std::string>(val); })
};

/**
 * 型に応じた文字列の型名を取得する
 *　@param[in] type 型情報
 *　@return 独自の型名
 *　@retval "object" 該当しない場合
 */
inline std::string getTypeName(const std::type_info& type) {
	std::string typeName (type.name());

	for (const AnyTypeTuple tuple : CONVERTER_MAP) {
		if (std::get<0>(tuple) == typeName) {
			return std::get<1>(tuple);
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
	for (const AnyTypeTuple tuple : CONVERTER_MAP) {
		if (std::get<1>(tuple) == typeName) {
			return std::get<2>(tuple)(val);
		}
	}
	throw std::invalid_argument("Could not convert to a boost::any.");
}


/***
 * boost の値を文字列に変換する
 * @param[in] val boost::any の値
 */
inline std::string booostAnyToString(const boost::any& val) {
	std::string typeName (val.type().name());

	for (const AnyTypeTuple tuple : CONVERTER_MAP) {
		if (std::get<0>(tuple) == typeName) {
			return std::get<3>(tuple)(val);
		}
	}

	throw std::invalid_argument("Could not convert and express to a string.");
}

}
}
}


#endif /* BOOSTANYCONVERTER_HPP_ */
