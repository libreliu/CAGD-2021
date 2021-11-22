#pragma once

#include "Common.h"
#include <vector>

namespace CAGD {

// A 2D bezier curve implementation
// Currently only 3 control point implemented
class QuadRationalBezierCurve {
private:

  // -- results & options --
  Vec<Vec3f> controlPoints;
  Vec<Vec2f> tessellatedCurve;
  bool needUpdate;
  // -------------

public:

  QuadRationalBezierCurve() : needUpdate(true) {}

  void setControlPoints(const Vec<Vec3f> &controlPoints) {
    this->needUpdate = true;
    this->controlPoints = controlPoints;
  }

  void resampleCurve(int numSamples = 100);

  const Vec<Vec2f> &getTessellatedCurve() {
    if (!needUpdate) {
      return tessellatedCurve;
    }

    resampleCurve();
    needUpdate = false;
    return tessellatedCurve;
  }
};

}

