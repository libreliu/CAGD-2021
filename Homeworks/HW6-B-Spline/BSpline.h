#pragma once

#include "Common.h"
#include <Eigen/Dense>
#include <vector>

namespace CAGD {

// A cubic bezier spline implementation
class BSpline {
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
  Eigen::VectorXd tKnots;
  Eigen::VectorXd sKnots;

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

  BSpline() : needUpdate(true) {}

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

  inline double computeN(
    const Eigen::VectorXd& tKnots,
    double t,
    int i,
    int k
  ) {
    if (k == 1) {
      // Search for knots
      int tIdx = -1;
      for (int idx = 0; idx < tKnots.size(); idx++) {
        if ((t >= tKnots(idx)) && 
        ((idx == tKnots.size() - 1) ? true : t <= tKnots(idx + 1))) {
          tIdx = idx;
        }
      }
      // tIdx represents the index for left endpoint
      assert(tIdx != -1);

      if (tIdx == i) {
        return 1.0;
      } else {
        return 0.0;
      }
    }

    double leftTerm = 0;
    {
      assert(i + k - 1 < tKnots.size());
      double dividend = tKnots(i+k-1) - tKnots(i);
      if (dividend == 0) {
        leftTerm = 0;
      } else {
        leftTerm = (t - tKnots(i)) / dividend * computeN(tKnots, t, i, k-1);
      }
    }

    double rightTerm = 0;
    {
      assert(i + k < tKnots.size());
      double dividend = tKnots(i+k) - tKnots(i+1);
      if (dividend == 0) {
        rightTerm = 0;
      } else {
        rightTerm = (tKnots(i+k) - t) / dividend * computeN(tKnots, t, i+1, k-1);
      }
    }

    if (k == 4) {
      printf("computeN: N_{%d,%d}(%lf) = %lf\n", i, k, t, leftTerm + rightTerm);
      printf("- tKnots used: ");
      for (int idx = 0; idx < tKnots.size(); idx++) {
        printf("%.5lf ", tKnots(idx));
      }
      printf("\n");
    }

    return leftTerm + rightTerm;
  }

  void calcControlPoints();
  void resampleSpline(int samplePerSegment = 5);

  inline const Vec<Vec2d> &getTessellatedCurve() {
    if (!needUpdate) {
      return tessellatedCurve;
    }

    calcControlPoints();
    resampleSpline();
    needUpdate = false;
    return tessellatedCurve;
  }

  inline const Vec<Vec2d> &getControlPoints() {
    if (!needUpdate) {
      return controlPoints;
    }

    calcControlPoints();
    resampleSpline();
    needUpdate = false;
    return controlPoints;
  }
};

}

