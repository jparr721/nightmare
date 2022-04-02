#include "d2V_linear_tetrahedron_dq2.h"
#include "../geometry.h"
#include "d2psi_neo_hookean_dF2.h"
#include "dphi_linear_tetrahedron_dX.h"

namespace nm::fem {
    auto d2VlinearTetrahedronDq2(const vecXr &q, const matXr &vertices, const vec4i &element, real mu, real lambda,
                                 real volume) -> mat1212r {
        // Obtain the deformed space vertex position matrix for this element
        mat34r deformed;
        for (int ii = 0; ii < 4; ++ii) { deformed.col(ii) = q.segment<3>(element(ii) * 3); }

        // Get the centroid of the deformed coordinates
        const vec3r centroid = computeTetrahedralCentroid(deformed);

        // Obtain the shape function gradient matrix, D.
        const mat43r dphiDX = dphiLinearTetrahedronDx(vertices, element, centroid);

        // Obtain the deformation gradient
        const mat3r F = deformed * dphiDX;

        // Obtain the hessian of the strain energy density function
        const mat99r d2PsiDf = d2PsiNeoHookeanDf2(F, mu, lambda);

        // Now, construct B_j from the shape function gradient matrix.
        mat912r B;
        for (int ii = 0; ii < 4; ++ii) {
            B.block(0, 0 + 3 * ii, 3, 1) = dphiDX.row(ii).transpose();
            B.block(3, 1 + 3 * ii, 3, 1) = dphiDX.row(ii).transpose();
            B.block(6, 2 + 3 * ii, 3, 1) = dphiDX.row(ii).transpose();
        }

        // Finally, get the hessian.
        mat1212r H = -volume * B.transpose() * d2PsiDf * B;

        // This code ensures that the hessian matrix is symmetric positive definite by projecting all
        // negative eigenvalues to small, positive values.
        Eigen::SelfAdjointEigenSolver<mat1212r> es(H);

        Eigen::MatrixXd diagEval = es.eigenvalues().real().asDiagonal();
        Eigen::MatrixXd evec = es.eigenvectors().real();

        for (int i = 0; i < 12; ++i) {
            if (es.eigenvalues()[i] < 1e-6) { diagEval(i, i) = 1e-3; }
        }

        H = evec * diagEval * evec.transpose();

        return H;
    }
}// namespace nm::fem