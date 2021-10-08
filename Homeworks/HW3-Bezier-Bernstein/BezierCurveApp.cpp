#include "imgui.h"
#include "implot.h"
#include "BezierCurveApp.h"
#include <cmath>
#include <string>

using namespace CAGD;

BezierCurveApp BezierCurveApp::inst;

void BezierCurveApp::onRender()
{
    bool needResample = false;

    ImGui::Begin("HW3 BezierCurve (Bernstein)");
    ImGui::Text("Homework 3 for Computer Aided Geometric Design 2021");

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

    ImGui::BulletText("Click and drag any point.");
    ImGui::Checkbox("Show Labels##2", &showLabels);
    ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks;
    //ImPlot::SetNextPlotLimits(0,1,0,1);
    if (ImPlot::BeginPlot("##Bezier", 0, 0, ImVec2(-1,0), ImPlotFlags_CanvasOnly | ImPlotFlags_AntiAliased, flags, flags)) {
        
        if (controlPoints.size() > 0) {
            ImPlot::PlotLine("controlPolygon",&controlPoints[0][0], &controlPoints[0][1], controlPoints.size(), 0, sizeof(Vec2d));

            for (size_t i = 0; i < controlPoints.size(); i++) {
                String pointName = "P" + std::to_string(i);
                bool changed = ImPlot::DragPoint(pointName.c_str(), &controlPoints[i].x(), &controlPoints[i].y(), showLabels);
                needResample = changed ? true : needResample;
            }

            if (needResample) {
                resample();
            }

            ImPlot::PlotLine("bezierCurve", &tessellatedCurve[0][0], &tessellatedCurve[0][1], tessellatedCurve.size(), 0, sizeof(Vec2d));
        }
        
        ImPlot::EndPlot();
    }

    ImGui::End();
}

void BezierCurveApp::resample(int numSamples) {
    double tStep = 1.0 / numSamples;
    size_t n = controlPoints.size();

    if (tessellatedCurve.size() < numSamples) {
        tessellatedCurve.resize(numSamples);
    }

    for (size_t sampleIdx = 0; sampleIdx < numSamples; sampleIdx++) {
        double t = tStep * sampleIdx;

        tessellatedCurve[sampleIdx] = Vec2d(0.0, 0.0); 
        for (int i = 0; i < n; i++) {
            // Will not work for n >= 20
            double C_n_i = factoralLUT.at(n-1) / factoralLUT.at(n - i-1) / factoralLUT.at(i);
            tessellatedCurve[sampleIdx] += controlPoints[i] * C_n_i * std::pow(t, i) * std::pow(1 - t, n -1- i);
        }
    }
}