/**
 * Property.hpp
 *
 * @date 2013/02/06
 * @author katsumata
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <boost/any.hpp>

#include "BoostAnyConverter.hpp"

namespace spd {
namespace core {

/**
 * プレイヤのプロパティを表すクラス
 */
class Property {

public:

	/**
	 * 出力のタイプ情報
	 */
	enum class OutputType { // 種類が2桁を越えるようになると、gexfOutput とかでエラーが起こる
		NOT, /**< 出力しない */
		COUNTABLE, /**< 値を加算し出力 */
		CLASSIFIABLE, /**< 値で分類し出力 */
	};

	/**
	 * 全てを設定して、初期化
	 * @param[in] name 名前
	 * @param[in] val 値
	 * @param[in] outputType 出力のタイプ
	 */
	Property(std::string name, boost::any val, OutputType outputType) : name(name), value(val), type(outputType) {}

	/**
	 * プロパティの名前を取得
	 * @return プロパティの名前
	 */
	const std::string& getName() const
	{
		return name;
	}

	/**
	 * 出力タイプを取得
	 * @return 出力タイプ
	 */
	OutputType getType() const {
		return type;
	}

	/**
	 * 値を設定
	 * @param[in] value 値
	 */
	void setValue(const boost::any& value) {
		this->value = value;
	}

	/**
	 * 値の型を指定して取得
	 * @return 値
	 * @throw boost::bad_any_cast 取得する時の型が適切でない場合
	 */
	template<typename T>
	T getValueAs() {
		try {
			return boost::any_cast<T>(value);
		} catch (std::exception& e) {
			std::cerr << "Could not get a value: " + this->name << std::endl;
			boost::throw_exception(boost::bad_any_cast());
		}
	}

	/**
	 * 値の型が指定した型か判定
	 * @return 値の型が指定した型かどうか
	 * @retval 指定した型の場合
	 * @retval 指定した型でない場合
	 */
	template<typename T>
	bool isValue() {
		if (value.type() == typeid(T)) {
			return true;
		}
		return false;
	}

	/**
	 * 型の名前を文字列にする
	 *
	 * @par
	 * BoostAnyConverter の一覧にしたがう
	 * @return 型の名前
	 */
	std::string valueType() {
		return converter::getTypeName(value.type());
	}

	/**
	 * 値を文字列として出力する
	 * @return 文字列にした値
	 * @throw std::runtime_error 文字列にできない場合
	 */
	std::string valueToString() {

		std::string result;
		try {
			result =  converter::booostAnyToString(value);
		} catch (std::exception& e) {
			throw std::runtime_error("Could not express a value as a string");
		}
		return result;
	}

private:

	// 名前
	std::string name;

	// 値
	boost::any value;

	// 出力情報
	OutputType type;

};

} /* namespace core */
} /* namespace spd */
#endif /* PROPERTY_H_ */
