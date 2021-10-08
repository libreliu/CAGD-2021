#pragma once

#include <vector>
#include <string>
#include "CommonMath.h"

namespace CAGD {
template<typename T>
using Vec = std::vector<T>;

using String = std::string;

class BezierCurveApp {
private:
  static BezierCurveApp inst;

  Vec<Vec2d> controlPoints;
  Vec<Vec2d> tessellatedCurve;

  Vec<Vec2d> intermediates[2];

  // GUI Related
  bool showLabels;

public:
  static inline BezierCurveApp &getInstance() {
    return inst;
  }
  BezierCurveApp& operator=(const BezierCurveApp&) = delete;

  // Should be called inside ImGui context, will launch a separate window
  void onRender();
  void resample(int numSamples = 100);
};
}