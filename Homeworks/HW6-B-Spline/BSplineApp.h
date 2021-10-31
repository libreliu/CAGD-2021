#pragma once

#include <vector>
#include <string>
#include "Common.h"
#include "BSpline.h"

namespace CAGD
{

  class BSplineApp
  {
  private:
    static BSplineApp inst;

    Vec<Vec2d> controlPoints;

    // GUI Related
    bool showLabels;
    bool showControlKnots;

    BSpline bSpline;

  public:
    static inline BSplineApp &getInstance()
    {
      return inst;
    }
    BSplineApp &operator=(const BSplineApp &) = delete;

    // Should be called inside ImGui context, will launch a separate window
    void onRender();
  };
}