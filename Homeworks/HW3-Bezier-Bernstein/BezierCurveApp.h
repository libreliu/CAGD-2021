#pragma once

#include <vector>
#include <string>
#include "CommonMath.h"

namespace CAGD
{
  template <typename T>
  using Vec = std::vector<T>;

  using String = std::string;

  class BezierCurveApp
  {
  private:
    static BezierCurveApp inst;

    Vec<Vec2d> controlPoints;
    Vec<Vec2d> tessellatedCurve;

    const Vec<double> factoralLUT = {
        1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600,
        6227020800, 87178291200, 1307674368000, 20922789888000, 355687428096000, 
        6402373705728000, 121645100408832000
    };

    // GUI Related
    bool showLabels;

  public:
    static inline BezierCurveApp &getInstance()
    {
      return inst;
    }
    BezierCurveApp &operator=(const BezierCurveApp &) = delete;

    // Should be called inside ImGui context, will launch a separate window
    void onRender();
    void resample(int numSamples = 100);
  };
}