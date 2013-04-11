/**
 * IShowParameter.hpp
 *
 * @date 2013/01/07
 * @author katsumata
 */

#ifndef ISHOWPARAMETER_H_
#define ISHOWPARAMETER_H_

#include <ostream>

namespace spd {
namespace param {

/**
 * パラメタを出力する抽象クラス
 */
class IShowParameter {
public:

	/**
	 * デストラクタ
	 */
	virtual ~IShowParameter() = default;

	/**
	 * パラメタを出力する
	 * @param[in] out 出力先
	 */
	virtual void showParameter(std::ostream& out) const = 0;
};

} /* namespace param */
} /* namespace spd */
#endif /* ISHOWPARAMETER_H_ */
