/**
 * SerializeProperty.hpp
 *
 * @date 2013/02/07
 * @author katsumata
 */

#ifndef SERIALIZEPROPERTY_HPP_
#define SERIALIZEPROPERTY_HPP_

#include <string>

#include "Serializer.hpp"
#include "../Property.hpp"

namespace spd {
namespace core {
namespace serialize {

class SerializedProperty {

public:

	/**
	 * デフォルトコンストラクタ
	 */
	SerializedProperty() : outputType(0) {}

	/**
	 * プロパティクラスからのコピーコンストラクタ
	 * @param[in] 元となるプロパティ
	 */
	SerializedProperty(Property& prop) {

		name = prop.getName();

		val = prop.valueToString();

		valType = prop.valueType();

		outputType = static_cast<int>(prop.getType());
	}

	/**
	 * プロパティ名を取得する
	 * @return プロパティ名
	 */
	const std::string& getName() const {
		return name;
	}

	/**
	 * プロパティ値を取得する
	 * @return プロパティ値
	 */
	const std::string& getVal() const {
		return val;
	}

	/**
	 * プロパティ値の型を取得する
	 * @return プロパティ値の型
	 */
	const std::string& getValType() const {
		return valType;
	}

	/**
	 * 出力のタイプを取得する
	 * @return 出力のタイプ
	 */
	int getOutputType() const {
		return outputType;
	}

	/**
	 * シリアライズ
	 */
	MSGPACK_DEFINE(name, val, valType, outputType);

private:

	std::string name;

	std::string val;
	std::string valType;

	int outputType;
};


} /* namespace serialize */
} /* namespace core */
} /* namespace spd */
#endif /* SERIALIZEPROPERTY_HPP_ */
