#pragma once

#include "Common.h"
#include <vector>

namespace CAGD {

// A cubic bezier spline implementation
class BezierSpline {
public:
  enum class ParamOpts {
    UNIFORM
  };
  enum class BoundaryOpts {
    NATURAL
  };


private:

  // -- results & options --
  Vec<Vec2d> knots;
  Vec<Vec2d> controlPoints;
  Vec<Vec2d> tessellatedCurve;
  enum class ParamOpts paramOpts;
  enum class BoundaryOpts boundaryOpts;
  bool needUpdate;

  // -------------

  // Helper for constructing linear equations - local to global point idx
  // bezierIdx: 0 ~ n-1
  // localIdx: 0 ~ 3
  inline int bezierL2G(int bezierIdx, int localIdx) {
    return bezierIdx * 3 + localIdx;
  }

public:

  BezierSpline() : needUpdate(true) {}

  inline const Vec<Vec2d> &getKnots() const {
    return knots;
  }

  inline void setKnots(const Vec<Vec2d>& knots) {
    this->knots = knots;
    needUpdate = true;
  }

  inline void setParamOpts(enum class ParamOpts pOpts) {
    if (pOpts != paramOpts) {
      paramOpts = pOpts;
      needUpdate = true;
    }
  }

  inline void setBoundaryOpts(enum class BoundaryOpts bOpts) {
    if (bOpts != boundaryOpts) {
      boundaryOpts = bOpts;
      needUpdate = true;
    }
  }

  void calcControlPoints();
  void resampleSpline(int samplePerSegment = 100);

  const Vec<Vec2d> &getTessellatedCurve() {
    if (!needUpdate) {
      return tessellatedCurve;
    }

    calcControlPoints();
    resampleSpline();
    needUpdate = false;
    return tessellatedCurve;
  }
};

}

