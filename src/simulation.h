#pragma once

#include "fem/V_linear_tetrahedron.h"
#include "fem/assemble.h"
#include "fem/spring_potential.h"
#include "mesh.h"
#include "mouse_control.h"
#include "nm_math.h"
#include "mesh.h"
#include <igl/opengl/glfw/Viewer.h>
#include <vector>

namespace nm {
    void simulate(vecXr &q, vecXr &qdot, real dt);
    auto setupVariables(vecXr &q, vecXr &qdot, Mesh &mesh) -> bool;
}// namespace nm