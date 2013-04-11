/**
 * OutputVisitor.cpp
 *
 * @date 2012/12/19
 * @author katsumata
 */

#include <stdexcept>
#include "OutputVisitor.hpp"
#include "../topology/AllTopology.hpp"
#include "../core/Space.hpp"
#include "../param/Parameter.hpp"
#include "../param/NeighborhoodParameter.hpp"

namespace spd {
namespace output {

OutputVisitor::~OutputVisitor() {}

/*
 * 構造によって処理を変更する
 * @param[in] 空間情報
 */
std::pair<std::string, bool> OutputVisitor::output(spd::core::Space& space) {
	space.getParameter().getNeighborhoodParameter()->getTopology()->accept(*this, space);


	// いまのとこないので空
	return std::pair<std::string, bool> {std::string(""), false};
}

} /* namespace output */
} /* namespace spd */
