#pragma once

#include <vector>
#include <string>
#include "Common.h"
#include "BezierCurve.h"

namespace CAGD
{

  class HomeworkApp
  {
  private:
    static HomeworkApp inst;

    Vec<Vec2d> controlPoints;

    // GUI Related
    bool showLabels;

    QuadRationalBezierCurve ellipseCurves[2];
    QuadRationalBezierCurve hyperbolaCurves[2];

    // Problem 1 related
    float prob1_x[3];
    float prob1_d;

    // Problem 2 related
    float prob2_a;
    float prob2_b;
    bool drawHyperbola;
    bool drawEllipse;

    HomeworkApp() {
      prob1_d = 1.0f;
      prob2_a = 1;
      prob2_b = 1;
      drawHyperbola = false;
      drawEllipse = false;
    }

  public:
    static inline HomeworkApp &getInstance()
    {
      return inst;
    }
    HomeworkApp &operator=(const HomeworkApp &) = delete;

    // Should be called inside ImGui context, will launch a separate window
    void onRender();

    void onProblem1();
    void onProblem2();
    void onProblem3();
  };
}