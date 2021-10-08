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

    ImGui::Begin("HW2 BezierCurve (de-Casteljau)");
    ImGui::Text("Homework 2 for Computer Aided Geometric Design 2021");

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
    for (int i = 0; i < 2; i++) {
        if (intermediates[i].size() < controlPoints.size()) {
            intermediates[i].resize(controlPoints.size());
        }
    }
    if (tessellatedCurve.size() < numSamples) {
        tessellatedCurve.resize(numSamples);
    }


    for (size_t sampleIdx = 0; sampleIdx < numSamples; sampleIdx++) {
        double t = tStep * sampleIdx;
        for (size_t layer = 0; layer < controlPoints.size(); layer++) {
            for (size_t i = layer; i < controlPoints.size(); i++) {
                if (layer == 0) {
                    intermediates[0][i] = controlPoints[i];
                } else {
                    intermediates[layer % 2][i] = intermediates[(layer + 1) % 2][i-1] * (1-t) + intermediates[(layer + 1) % 2][i] * t;
                }
            }
        }
        
        tessellatedCurve[sampleIdx] = intermediates[(controlPoints.size() - 1) % 2][controlPoints.size() - 1];
    }
}