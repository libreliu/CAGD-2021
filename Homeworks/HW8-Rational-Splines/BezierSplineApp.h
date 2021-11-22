#pragma once

#include <vector>
#include <string>
#include "Common.h"
#include "BezierSpline.h"

namespace CAGD
{

  class BezierSplineApp
  {
  private:
    static BezierSplineApp inst;

    Vec<Vec2d> controlPoints;

    // GUI Related
    bool showLabels;

    BezierSpline bezSpline;

    // Problem 1 related
    float prob1_x[3];
    float prob1_d;

  public:
    static inline BezierSplineApp &getInstance()
    {
      return inst;
    }
    BezierSplineApp &operator=(const BezierSplineApp &) = delete;

    // Should be called inside ImGui context, will launch a separate window
    void onRender();

    void onProblem1();
    void onProblem2();
    void onProblem3();
  };
}