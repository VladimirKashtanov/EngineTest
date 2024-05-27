#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include "TestGLFWCore/Application.hpp"


class TestGLFWEditor : public TestGLFW::Application
{
	void on_update() override
	{
		//std::cout << "Update frame: " << frame++ << std::endl;
	}


	void on_ui_draw() override
	{
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("camera position", camera_position, -10.0f, 10.0f);
		ImGui::SliderFloat3("camera rotation", camera_rotation, 0.0f, 360.0f);
		ImGui::Checkbox("Perspective camera", &perspective_camera);
		ImGui::End();
	}


	int frame = 0;
};


int main()
{
	auto pTestGLFWEditor = std::make_unique<TestGLFWEditor>();

	int returnCode = pTestGLFWEditor->start(1024, 768, "TestGLFW Editor");

	return returnCode;
}