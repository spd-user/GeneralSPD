/**
 * Lattice.cpp
 *
 * @date 2012/10/05
 * @author katsumata
 */

#include "Lattice.hpp"

namespace spd {
namespace topology {

Lattice::Lattice() : side(0) {};

Lattice::~Lattice() {}

int Lattice::getSide() const {
	return this->side;
}

} /* namespace topology */
} /* namespace spd */
