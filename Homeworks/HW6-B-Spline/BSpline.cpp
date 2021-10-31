#include "BSpline.h"
#include <Eigen/Dense>
#include <cassert>
#include <iostream>

using namespace CAGD;

void BSpline::calcControlPoints() {
  if (knots.size() < 3) {
    return;
  }

  // number of b spline curves
  int n = knots.size() - 1;
  Eigen::MatrixXd A(n + 3, n + 3);
  A.setZero();
  Eigen::MatrixXd b(n + 3, 2); // [b_x, b_y]
  b.setZero();

  // Generate parameterizations
  tKnots = Eigen::VectorXd(n+7);
  sKnots = Eigen::VectorXd(n+1);
  
  switch (paramOpts) {
    case ParamOpts::UNIFORM:
    // t0 = t1 = t2 = t3 = 0
    for (int i = 0; i < 4; i++) {
      tKnots(i) = 0;
    }

    // t_{k} = 1, t_{k+1} = 2, ..., t_{k+n-2} = n-1
    for (int i = 0; i + 2 <= n; i++) {
      tKnots(i + 4) = i+1;
    }

    tKnots(n+6) = tKnots(n+5) = tKnots(n+4) = tKnots(n+3) = n;

    for (int i = 0; i < n+1; i++) {
      sKnots(i) = i;
    }
    break;
  }

  int equationIdx = 0;
  // Inner equations
  {
    for (int i = 0; i < n+1; i++) {
      if (i == 0) {
        A(equationIdx, 0) = 1;
      } else if (i == n) {
        A(equationIdx, n+2) = 1;
      } else {
        A(equationIdx, i) = computeN(tKnots, sKnots(i), i, 4);
        A(equationIdx, i+1) = computeN(tKnots, sKnots(i), i+1, 4);
        A(equationIdx, i+2) = computeN(tKnots, sKnots(i), i+2, 4);
      }

      b(equationIdx, 0) = knots[i].x();
      b(equationIdx, 1) = knots[i].y();
      equationIdx++;
    }
  }

  // Boundary equations
  {
    A(equationIdx, 0) = sKnots(2) - sKnots(0);
    A(equationIdx, 1) = -(sKnots(2) - sKnots(0))-(sKnots(1)-sKnots(0));
    A(equationIdx, 2) = sKnots(1) - sKnots(0);
    equationIdx++;

    A(equationIdx, n) = sKnots(n) - sKnots(n-1);
    A(equationIdx, n+1) = -(sKnots(n)-sKnots(n-1))-(sKnots(n)-sKnots(n-2));
    A(equationIdx, n+2) = sKnots(n) - sKnots(n-2);
    equationIdx++;
  }


  // Total 3n+1 equations
  assert(equationIdx == n+3);
  std::cout << A << std::endl;
  std::cout << b << std::endl;

  Eigen::MatrixXd x = A.colPivHouseholderQr().solve(b);
  
  // fill points
  controlPoints.resize(n+3);
  for (int i = 0; i < n+3; i++) {
    controlPoints[i].x() = x(i, 0);
    controlPoints[i].y() = x(i, 1);
  }
}

void BSpline::resampleSpline(int samplePerSegment) {
  if (knots.size() < 3) {
    return;
  }

  int n = knots.size() - 1;
  tessellatedCurve.clear();

  int totalSamples = samplePerSegment * n;
  for (int i = 0; i < totalSamples; i++) {
    double tStep = ((double)i / (totalSamples - 1)) * n;
    Vec2d xSum(0.0, 0.0);
    for (int idx = 0; idx <= n + 2; idx++) {
      xSum += controlPoints[idx] * computeN(tKnots, tStep, idx, 4);
    }
    tessellatedCurve.push_back(xSum);
  }

}