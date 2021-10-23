#include "BezierSpline.h"
#include <Eigen/Dense>
#include <cassert>

using namespace CAGD;

void BezierSpline::calcControlPoints() {
  assert(knots.size() >= 2);

  // number of bezier curves
  int n = knots.size() - 1;
  Eigen::MatrixXd A(3*n + 1, 3*n + 1);
  A.setZero();
  Eigen::MatrixXd b(3*n + 1, 2); // [b_x, b_y]
  b.setZero();

  // Generate parameterizations
  Eigen::VectorXd tKnots(n+1);
  
  switch (paramOpts) {
    case ParamOpts::UNIFORM:
    for (int i = 0; i < n+1; i++) {
      tKnots(i) = i;
    }

    break;
  }

  int equationIdx = 0;
  // C^0 conditions, 0 ~ n (n+1 total)
  {
    for (int i = 0; i < n; i++) {
      A(equationIdx, bezierL2G(i, 0)) = 1;
      b(equationIdx, 0) = knots[i].x();
      b(equationIdx, 1) = knots[i].y();
      equationIdx++;
    }

    A(equationIdx, bezierL2G(n-1, 3)) = 1;
    b(equationIdx, 0) = knots[n].x();
    b(equationIdx, 1) = knots[n].y();
    equationIdx++;
  }

  // C^1 conditions, n+1 ~ 2n
  {
    for (int i = 1; i < n; i++) {
      A(equationIdx, bezierL2G(i-1, 3)) += 1.0 / (tKnots(i) - tKnots(i-1));
      A(equationIdx, bezierL2G(i-1, 2)) += - 1.0 / (tKnots(i) - tKnots(i-1));

      A(equationIdx, bezierL2G(i, 1)) += - 1.0 / (tKnots(i+1) - tKnots(i));
      A(equationIdx, bezierL2G(i, 0)) += 1.0 / (tKnots(i+1) - tKnots(i));
      b(equationIdx, 0) = b(equationIdx, 1) = 0;
      equationIdx++;
    }
  }

  // C^2 conditions, 2n ~ 3n-1
  {
    for (int i = 1; i < n; i++) {
      A(equationIdx, bezierL2G(i-1, 3)) += 1.0 / (tKnots(i) - tKnots(i-1));
      A(equationIdx, bezierL2G(i-1, 2)) += - 2.0 / (tKnots(i) - tKnots(i-1));
      A(equationIdx, bezierL2G(i-1, 1)) += 1.0 / (tKnots(i) - tKnots(i-1));

      A(equationIdx, bezierL2G(i, 2)) += - 1.0 / (tKnots(i+1) - tKnots(i));
      A(equationIdx, bezierL2G(i, 1)) += 2.0 / (tKnots(i+1) - tKnots(i));
      A(equationIdx, bezierL2G(i, 0)) += - 1.0 / (tKnots(i+1) - tKnots(i));
      b(equationIdx, 0) = b(equationIdx, 1) = 0;
      equationIdx++;
    }
  }

  // Boundary conditions, 3n ~ 3n+1
  switch (boundaryOpts) {
    case BoundaryOpts::NATURAL: {
      A(equationIdx, bezierL2G(0, 2)) = 1;
      A(equationIdx, bezierL2G(0, 1)) = -2;
      A(equationIdx, bezierL2G(0, 0)) = 1;
      b(equationIdx, 0) = b(equationIdx, 1) = 0;
      equationIdx++;

      A(equationIdx, bezierL2G(n-1, 1)) = 1;
      A(equationIdx, bezierL2G(n-1, 2)) = -2;
      A(equationIdx, bezierL2G(n-1, 3)) = 1;
      b(equationIdx, 0) = b(equationIdx, 1) = 0;
      equationIdx++;
    }
    break;
  }

  // Total 3n+2 equations
  assert(equationIdx == 3*n+2);

  
}

void BezierSpline::resampleSpline(int samplePerSegment) {
  // do this
}