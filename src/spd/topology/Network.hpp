/**
 * Network.hpp
 *
 * @date 2012/12/19
 * @author katsumata
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "Topology.hpp"

namespace spd {
namespace topology {

/**
 * を表すクラス
 */
class Network: public spd::topology::Topology {
public:
	Network();
	virtual ~Network() = default;
};

} /* namespace topology */
} /* namespace spd */
#endif /* NETWORK_H_ */
