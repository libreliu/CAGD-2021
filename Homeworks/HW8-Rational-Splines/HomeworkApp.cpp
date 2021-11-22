#include "imgui.h"
#include "implot.h"
#include "HomeworkApp.h"
#include "BezierCurve.h"
#include <cmath>
#include <string>

using namespace CAGD;

HomeworkApp HomeworkApp::inst;

void HomeworkApp::onProblem1() {
    ImGui::Begin("HW8 Rational Bezier Curves - Problem 1");
    
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

void HomeworkApp::onProblem2() {
    ImGui::Begin("HW8 Rational Bezier Curves - Problem 2");

    bool needUpdate = (ImGui::DragFloat("a", &prob2_a, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f"));
    
    needUpdate = (ImGui::DragFloat("b", &prob2_b, 0.1f, 0.f, std::numeric_limits<float>::max(), "%.2f")) ? true : needUpdate;

    ImGui::Checkbox("Ellipse", &drawEllipse);
    ImGui::SameLine();
    ImGui::Checkbox("Hyperbola", &drawHyperbola);

    //if (needUpdate) {
        if (drawEllipse) {
            std::vector<Vec3f> controlPoints {
                Vec3f(prob2_a, 0, 1),
                Vec3f(prob2_a, prob2_b, 1),
                Vec3f(0, 2 * prob2_b, 2),
            };
            std::vector<Vec3f> controlPointsDual {
                Vec3f(prob2_a, 0, 1),
                Vec3f(-prob2_a, -prob2_b, -1),
                Vec3f(0, 2 * prob2_b, 2),
            };

            ellipseCurves[0].setControlPoints(controlPoints);
            ellipseCurves[1].setControlPoints(controlPointsDual);
        }

        if (drawHyperbola) {
            std::vector<Vec3f> controlPoints {
                Vec3f(prob2_a, 0, 1),
                Vec3f(prob2_a, prob2_b, 1),
                Vec3f(2 * prob2_a, 2 * prob2_b, 0),
            };
            std::vector<Vec3f> controlPointsDual {
                Vec3f(prob2_a, 0, 1),
                Vec3f(-prob2_a, -prob2_b, -1),
                Vec3f(2 * prob2_a, 2 * prob2_b, 0),
            };

            hyperbolaCurves[0].setControlPoints(controlPoints);
            hyperbolaCurves[1].setControlPoints(controlPointsDual);
        }
    //}

    ImPlotAxisFlags flags = ImPlotAxisFlags_None; // ImPlotAxisFlags_Lock;
    ImPlot::SetNextPlotLimits(-1,1,-1,1);
    if (ImPlot::BeginPlot("Curve", "x", "y", ImVec2(-1, -1), ImPlotFlags_AntiAliased, flags, flags)) {
        if (drawEllipse) {
            auto &tessEllipse = ellipseCurves[0].getTessellatedCurve();
            auto &tessEllipseDual = ellipseCurves[1].getTessellatedCurve();
            ImPlot::PlotLine("ellipseSeg", &tessEllipse[0][0], &tessEllipse[0][1], tessEllipse.size(), 0, sizeof(Vec2f));
            ImPlot::PlotLine("ellipseDualSeg", &tessEllipseDual[0][0], &tessEllipseDual[0][1], tessEllipseDual.size(), 0, sizeof(Vec2f));
        }

        if (drawHyperbola) {
            auto &tessHyperbola = hyperbolaCurves[0].getTessellatedCurve();
            auto &tessHyperbolaDual = hyperbolaCurves[1].getTessellatedCurve();
            ImPlot::PlotLine("hyperbolaSeg", &tessHyperbola[0][0], &tessHyperbola[0][1], tessHyperbola.size(), 0, sizeof(Vec2f));
            ImPlot::PlotLine("hyperbolaDualSeg", &tessHyperbolaDual[0][0], &tessHyperbolaDual[0][1], tessHyperbolaDual.size(), 0, sizeof(Vec2f));
        }

        ImPlot::EndPlot();
    }

    ImGui::End();
}

void HomeworkApp::onProblem3() {
    ImGui::Begin("HW8 Rational Bezier Curves - Problem 3");

    

    ImGui::End();
}

void HomeworkApp::onRender()
{
    onProblem1();
    onProblem2();
    onProblem3();
}