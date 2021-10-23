#pragma once

#include "Common.h"
#include <vector>

namespace CAGD {

// A cubic bezier spline implementation
class BezierSpline {
private:

  // -- results & options --
  Vec<Vec2d> knots;
  Vec<Vec<Vec2d>> controlPoints;
  Vec<Vec2d> tessellatedCurve;
  enum class ParamOpts {
    UNIFORM
  } paramOpts;
  enum class BoundaryOpts {
    NATURAL
  } boundaryOpts;
  bool needUpdate;

  // -------------

  // Helper for constructing linear equations - local to global point idx
  // bezierIdx: 0 ~ n-1
  // localIdx: 0 ~ 3
  inline int bezierL2G(int bezierIdx, int localIdx) {
    return bezierIdx * 3 + localIdx;
  }

public:
  BezierSpline() : needUpdate(false) {}

  inline const Vec<Vec2d> &getKnots() const {
    return knots;
  }

  inline void setKnots(const Vec<Vec2d>& knots) {
    this->knots = knots;
  }

  inline void setParamOpts(enum class ParamOpts pOpts) {
    paramOpts = pOpts;
  }

  inline void setBoundaryOpts(enum class BoundaryOpts bOpts) {
    boundaryOpts = bOpts;
  }

  void calcControlPoints();
  void resampleSpline(int samplePerSegment = 100);
};

}

