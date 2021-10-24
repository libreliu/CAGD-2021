#include "imgui.h"
#include "implot.h"
#include "BezierSplineApp.h"
#include "BezierSpline.h"
#include <cmath>
#include <string>

using namespace CAGD;

BezierSplineApp BezierSplineApp::inst;

void BezierSplineApp::onRender()
{
    bool needResample = false;

    ImGui::Begin("HW5 BezierSpline");
    ImGui::Text("Homework 5 for Computer Aided Geometric Design 2021");

    ImGui::Separator();

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