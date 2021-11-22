#include "imgui.h"
#include "implot.h"
#include "BezierSplineApp.h"
#include "BezierSpline.h"
#include <cmath>
#include <string>

using namespace CAGD;

BezierSplineApp BezierSplineApp::inst;

void BezierSplineApp::onProblem1() {
    ImGui::Begin("HW8 Rational Spline - Problem 1");
    
    ImGui::DragFloat3("x", &prob1_x[0], 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
    ImGui::DragFloat("d", &prob1_d, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f");

    std::vector<std::pair<Vec3f, Vec3f>> cubeLines = {
        {Vec3f(1, 1, 1), Vec3f(1, 1, -1)},
        {Vec3f(1, 1, -1), Vec3f(1, -1, -1)},
        {Vec3f(1, -1, -1), Vec3f(1, -1, 1)},
        {Vec3f(1, -1, 1), Vec3f(1, 1, 1)},

        {Vec3f(1, 1, 1), Vec3f(-1, 1, 1)},
        {Vec3f(1, 1, -1), Vec3f(-1, 1, -1)},
        {Vec3f(1, -1, -1), Vec3f(-1, -1, -1)},
        {Vec3f(1, -1, 1), Vec3f(-1, -1, 1)},

        {Vec3f(-1, 1, 1), Vec3f(-1, 1, -1)},
        {Vec3f(-1, 1, -1), Vec3f(-1, -1, -1)},
        {Vec3f(-1, -1, -1), Vec3f(-1, -1, 1)},
        {Vec3f(-1, -1, 1), Vec3f(-1, 1, 1)},
    };

    auto getWorldPosition = [](float x[], Vec3f modelPosition) {
        return Vec3f(modelPosition[0] + x[0], modelPosition[1] + x[1], modelPosition[2] + x[2]);
    };

    float z0 = -3;
    float nearZ = -1;

    auto getProjectedViewPosition = [z0, nearZ](Vec3f worldPosition) {
        Vec4f homo = worldPosition.cast_to_homo();
        homo[3] = (homo[2] - z0) * nearZ;
        auto non_homo = homo.cast_to_non_homo();
        return Vec2f(non_homo[0], non_homo[1]);
    };

    ImPlotAxisFlags flags = ImPlotAxisFlags_Lock;
    ImPlot::SetNextPlotLimits(-1,1,-1,1);
    if (ImPlot::BeginPlot("Curve", "x", "y", ImVec2(-1, -1), ImPlotFlags_AntiAliased, flags, flags)) {

        for (auto &line: cubeLines) {
            Vec3f worldPos[2];
            worldPos[0] = getWorldPosition(prob1_x, line.first * prob1_d);
            worldPos[1] = getWorldPosition(prob1_x, line.second * prob1_d);

            Vec2f projectedPos[2];
            projectedPos[0] = getProjectedViewPosition(worldPos[0]);
            projectedPos[1] = getProjectedViewPosition(worldPos[1]);

            ImPlot::PlotLine("line", &projectedPos[0].x(), &projectedPos[0].y(), 2, 0, sizeof(Vec2f));
        }
        
        ImPlot::EndPlot();
    }

    ImGui::End();
}

void BezierSplineApp::onProblem2() {

}

void BezierSplineApp::onProblem3() {

}

void BezierSplineApp::onRender()
{
    onProblem1();
    onProblem2();
    onProblem3();
}