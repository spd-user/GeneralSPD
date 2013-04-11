/**
 * IToString.hpp
 *
 * @date 2013/01/09
 * @author katsumata
 */

#ifndef ITOSTRING_H_
#define ITOSTRING_H_

#include <string>

namespace spd {

/**
 * toStringを提供するインターフェースクラス
 */
class IToString {
public:

	/**
	 * デストラクタ
	 */
	virtual ~IToString(){};

	/**
	 * クラス情報の文字出力
	 * @return クラス情報
	 */
	virtual std::string toString() const = 0;

};

} /* namespace spd */
#endif /* ITOSTRING_H_ */
