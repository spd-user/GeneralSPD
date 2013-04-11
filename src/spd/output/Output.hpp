/**
 * Output.hpp
 *
 * @date 2012/09/08
 * @author katsumata
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <utility>
#include "../IToString.hpp"

namespace spd {
namespace core {
class Space;
}
namespace param {
class Parameter;
}

namespace output {

/**
 * 出力方法を表す抽象クラス
 */
class Output : public IToString {
public:

	/**
	 * デストラクタ
	 */
	virtual ~Output() {};

	/**
	 * 空間の情報を出力する
	 * @param[in, out] space 空間
	 * @return 圧縮する必要がある出力かどうか
	 * @retval 圧縮する必要がある場合 second がtrueで、firstにファイル名
	 * @retval 圧縮する必要がない場合 second がfalseで、firstは空
	 */
	virtual std::pair<std::string, bool> output(spd::core::Space& space) = 0;

	/**
	 * 初期化を行う
	 * @param[in, out] space 空間
	 * @param[in, out] param パラメタ
	 */
	virtual void init(spd::core::Space& space, spd::param::Parameter& param) = 0;
};

} /* namespace core */
} /* namespace spd */
#endif /* OUTPUT_H_ */
