/**
 * Lattice.cpp
 *
 * @date 2012/10/05
 * @author katsumata
 */

#include "Lattice.hpp"

#include <cmath> //sqrt

#include "../../core/Space.hpp"
#include "../../output/OutputVisitor.hpp"

namespace spd {
namespace topology {

Lattice::Lattice() : side(0) {};

Lattice::~Lattice() {}

int Lattice::getSide() const {
	return this->side;
}

void Lattice::accept(spd::output::OutputVisitor& visitor, spd::core::Space& space) {
	this->side = static_cast<int>(std::sqrt(space.getPlayers().size()));
	visitor.output(*this, space);
}

} /* namespace topology */
} /* namespace spd */
