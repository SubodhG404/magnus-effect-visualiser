#include "controlpanel.h"
#include "imgui.h"

ControlPanel::ControlPanel(Ball& b, Simulation& sim, Input& in, Trail& t)
    : ball(b), simulation(sim), input(in), trail(t), showPanel(true) {
}

void ControlPanel::draw(float uiWidth, float totalWindowWidth, float windowHeight) {
    if (!showPanel) return;

    float viewWidth = totalWindowWidth - uiWidth;
    float panelX = viewWidth + 5;
    float panelWidth = uiWidth - 15;
    float panelHeight = windowHeight - 20;
    float panelY = 10;

    ImGui::SetNextWindowPos(ImVec2(panelX, panelY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight), ImGuiCond_Always);

    ImGui::Begin("Controls", &showPanel,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetWindowSize(ImVec2(panelWidth, panelHeight));

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

    ImGui::SetWindowFontScale(1.2f);

    ImGui::Text("MAGNUS EFFECT");
    ImGui::Separator();

    float w = ImGui::GetContentRegionAvail().x;

    ImGui::Text("Velocity (m/s)");
    ImGui::InputFloat("VX", &input.velocityX, 0, 0, "%.1f");
    ImGui::InputFloat("VY", &input.velocityY, 0, 0, "%.1f");
    ImGui::InputFloat("VZ", &input.velocityZ, 0, 0, "%.1f");

    ImGui::Separator();
    ImGui::Text("Spin");
    ImGui::InputFloat("RPM", &input.spinRPM, 0, 0, "%.0f");
    ImGui::InputFloat("Axis X", &input.spinAxisX, 0, 0, "%.2f");
    ImGui::InputFloat("Axis Y", &input.spinAxisY, 0, 0, "%.2f");
    ImGui::InputFloat("Axis Z", &input.spinAxisZ, 0, 0, "%.2f");

    ImGui::Separator();
    ImGui::Text("Physics");
    ImGui::InputFloat("Mass (kg)", &ball.mass, 0, 0, "%.2f");
    ImGui::InputFloat("Radius (m)", &ball.radius, 0, 0, "%.3f");

    ImGui::Separator();
    ImGui::Text("Start Position");
    ImGui::InputFloat("X", &input.startPosX, 0, 0, "%.1f");
    ImGui::InputFloat("Y", &input.startPosY, 0, 0, "%.1f");
    ImGui::InputFloat("Z", &input.startPosZ, 0, 0, "%.1f");

    if (!simulation.isLaunched) {
        glm::vec3 newPos(input.startPosX, input.startPosY, input.startPosZ);
        ball.position = newPos;
        simulation.referenceBall.position = newPos;
    }

    ImGui::Separator();
    ImGui::Text("Simulation");
    ImGui::InputFloat("Speed Scale", &simulation.timeScale, 0, 0, "%.1f");

    ImGui::Separator();
    bool trailOn = trail.enabled;
    if (ImGui::Checkbox("Show Trails", &trailOn)) trail.enabled = trailOn;
    ImGui::Indent();
    bool magnusTrail = trail.magnusEnabled;
    if (ImGui::Checkbox("With Magnus", &magnusTrail)) trail.magnusEnabled = magnusTrail;
    ImGui::Indent();
    bool magTraj = trail.magnusTrajectoryEnabled;
    if (ImGui::Checkbox("  Trajectory (Yellow)", &magTraj)) trail.magnusTrajectoryEnabled = magTraj;
    bool magGround = trail.magnusGroundEnabled;
    if (ImGui::Checkbox("  Ground (Orange)", &magGround)) trail.magnusGroundEnabled = magGround;
    ImGui::Unindent();
    bool refTrail = trail.referenceEnabled;
    if (ImGui::Checkbox("Without Magnus", &refTrail)) trail.referenceEnabled = refTrail;
    ImGui::Indent();
    bool refTraj = trail.referenceTrajectoryEnabled;
    if (ImGui::Checkbox("  Trajectory (Gray)", &refTraj)) trail.referenceTrajectoryEnabled = refTraj;
    bool refGround = trail.referenceGroundEnabled;
    if (ImGui::Checkbox("  Ground (Light)", &refGround)) trail.referenceGroundEnabled = refGround;
    ImGui::Unindent();
    ImGui::Unindent();

    ImGui::Separator();

    ImVec2 btnSize(w / 2 - 4, 40);

    if (ImGui::Button("LAUNCH", btnSize)) {
        input.launch();
    }
    ImGui::SameLine();
    if (ImGui::Button("RESET", btnSize)) {
        input.reset();
    }

    ImGui::Separator();
    ImGui::Text("State");
    if (simulation.isLaunched) {
        ImGui::Text("Speed: %.1f m/s", simulation.getSpeed());
        ImGui::Text("Pos: (%.1f, %.1f, %.1f)", ball.position.x, ball.position.y, ball.position.z);
        ImGui::Text("Magnus: %.3f N", simulation.getMagnusForceMagnitude());
    } else {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Ready");
    }

    ImGui::Separator();
    ImGui::Text("Legend");
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Yellow: With Magnus");
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Gray: Without Magnus");
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Red: Magnus Force");

    ImGui::PopStyleVar(2);

    ImGui::End();
}
