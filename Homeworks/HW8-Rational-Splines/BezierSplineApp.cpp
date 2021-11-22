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

    auto getProjectedViewPosition = [](float z0, float nearZ, Vec3f worldPosition) {
        Vec4f homo = worldPosition.cast_to_homo();
        homo[3] = (homo[2] - z0) * nearZ;
        return homo.cast_to_non_homo();
    };

    ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks;
    //ImPlot::SetNextPlotLimits(0,1,0,1);
    if (ImPlot::BeginPlot("Curve", "x", "y", ImVec2(-1, -1), ImPlotFlags_CanvasOnly | ImPlotFlags_AntiAliased, flags, flags)) {

        for (auto &line: cubeLines) {
            Vec3f worldPos[2];
            worldPos[0] = getWorldPosition(prob1_x, line.first * prob1_d);
            worldPos[1] = getWorldPosition(prob1_x, line.second * prob1_d);

            
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

    bool needResample = false;

    ImGui::Begin("HW8 Rational Spline - Problem 1");


    ImGui::Text("Number of control points: %d", controlPoints.size());
    ImGui::Text("Control points:");
    ImGui::SameLine();
    if (ImGui::Button("Add")) {
        controlPoints.push_back(Vec2d(0.1 * controlPoints.size(), 0.1 * controlPoints.size()));
        needResample = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove") && controlPoints.size() > 0) {
        controlPoints.pop_back();
        needResample = true;
    }

    ImGui::BulletText("NEW: Hold #Ctrl and click to add point");
    ImGui::Checkbox("Show Labels##2", &showLabels);
    ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks;
    //ImPlot::SetNextPlotLimits(0,1,0,1);
    if (ImPlot::BeginPlot("Curve", "x", "y", ImVec2(-1, -1), ImPlotFlags_CanvasOnly | ImPlotFlags_AntiAliased, flags, flags)) {

        // Add point with Control
        if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl) {
            ImPlotPoint pt = ImPlot::GetPlotMousePos();
            controlPoints.push_back(Vec2d(pt.x, pt.y));
            needResample = true;
        }

        if (controlPoints.size() > 0) {
            ImPlot::PlotLine("controlPolygon",&controlPoints[0][0], &controlPoints[0][1], controlPoints.size(), 0, sizeof(Vec2d));

            for (size_t i = 0; i < controlPoints.size(); i++) {
                String pointName = "P" + std::to_string(i);
                bool changed = ImPlot::DragPoint(pointName.c_str(), &controlPoints[i].x(), &controlPoints[i].y(), showLabels);
                needResample = changed ? true : needResample;
            }

            if (needResample) {
                bezSpline.setBoundaryOpts(BezierSpline::BoundaryOpts::NATURAL);
                bezSpline.setParamOpts(BezierSpline::ParamOpts::UNIFORM);
                bezSpline.setKnots(controlPoints);
            }

            auto &tessellatedCurve = bezSpline.getTessellatedCurve();
            if (tessellatedCurve.size() > 0) {
                ImPlot::PlotLine("bezierCurve", &tessellatedCurve[0][0], &tessellatedCurve[0][1], tessellatedCurve.size(), 0, sizeof(Vec2d));
            }
        }
        
        ImPlot::EndPlot();
    }

    ImGui::End();
}