#pragma once

#include "../math.h"
#include <string>

namespace nm::geometry {
    class TetMesh {
        enum TetgenResult {
            kTetgenFailedToConvert = -1,
            kTetgenCrashed = 1,
            kTetgenFailedToCreateTets = 2,
            kTetgenFailedToConvertToMatrix = 3,
        };

    public:
        TetMesh() = default;
        explicit TetMesh(const std::string &meshPath);
        TetMesh(const mat &vertices, const mat &tetrahedra);

        void setVertices(const mat &vertices);
        void setTetrahedra(const mati &tetrahedra);

    private:
        mat vertices_;
        mati tetrahedra_;
        mati faces_;

        void tetrahedralizeMesh();
    };
}// namespace nm::geometry