#include "BezierCurve.h"
#include <Eigen/Dense>
#include <cassert>

using namespace CAGD;

void QuadRationalBezierCurve::resampleCurve(int numSamples) {
  if (controlPoints.size() != 3) {
    return;
  }

  int n = (int)controlPoints.size() - 1;
  Vec2f p[3];
  p[0] = Vec2f(controlPoints[0].x(), controlPoints[0].y());
  p[1] = Vec2f(controlPoints[1].x(), controlPoints[1].y());
  p[2] = Vec2f(controlPoints[2].x(), controlPoints[2].y());

  tessellatedCurve.clear();
  for (int tStep = 0; tStep < numSamples; tStep++) {
    float t = ((float)tStep) / numSamples;
    float denominator = controlPoints[0][2] * (1-t)*(1-t) 
                      + controlPoints[1][2] * 2 * t*(1-t)
                      + controlPoints[2][2] * t*t;
    tessellatedCurve.push_back(
      (p[0] * (1-t)*(1-t) + p[1] * 2 * t*(1-t) + p[2] * t*t) / denominator
    );
  }
}