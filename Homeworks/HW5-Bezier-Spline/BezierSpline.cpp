#include "BezierSpline.h"
#include <Eigen/Dense>
#include <cassert>

using namespace CAGD;

void BezierSpline::calcControlPoints() {
  if (knots.size() < 2) {
    return;
  }

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

  // C^1 conditions, n+1 ~ 2n-1 (n-1 total)
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

  // C^2 conditions, 2n ~ 3n-2 (n-1 total)
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

  // Boundary conditions, 3n-1 ~ 3n (2 total)
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

  // Total 3n+1 equations
  assert(equationIdx == 3*n+1);

  Eigen::MatrixXd x = A.colPivHouseholderQr().solve(b);
  
  // fill points
  controlPoints.resize(3*n+1);
  for (int i = 0; i < 3*n+1; i++) {
    controlPoints[i].x() = x(i, 0);
    controlPoints[i].y() = x(i, 1);
  }
}

void BezierSpline::resampleSpline(int samplePerSegment) {
  if (knots.size() < 2) {
    return;
  }

  int n = knots.size() - 1;
  tessellatedCurve.clear();
  for (int segmentIdx = 0; segmentIdx < n; segmentIdx++) {
    Vec2d p[4] = {
      controlPoints[bezierL2G(segmentIdx, 0)],
      controlPoints[bezierL2G(segmentIdx, 1)],
      controlPoints[bezierL2G(segmentIdx, 2)],
      controlPoints[bezierL2G(segmentIdx, 3)]
    };

    for (int tStep = 0; tStep < samplePerSegment; tStep++) {
      double t = ((double)tStep) / samplePerSegment;
      tessellatedCurve.push_back(
        p[0] * (1-t)*(1-t)*(1-t) + p[1] * 3 * t*(1-t)*(1-t) + p[2] * 3 * t*t*(1-t) + p[3] * t*t*t
      );
    }
  }
}