#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <TestGLFWCore/Input.hpp>
#include <TestGLFWCore/Application.hpp>


class TestGLFWEditor : public TestGLFW::Application
{
	void on_update() override
	{
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_W))
		{
			camera_position[2] -= 0.001f;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_S))
		{
			camera_position[2] += 0.001f;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_A))
		{
			camera_position[0] -= 0.001f;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_D))
		{
			camera_position[0] += 0.001f;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_E))
		{
			camera_position[1] += 0.001f;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_Q))
		{
			camera_position[1] -= 0.001f;
		}

		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_UP))
		{
			camera_rotation[0] += 0.1f;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_DOWN))
		{
			camera_rotation[0] -= 0.1f;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_RIGHT))
		{
			camera_rotation[1] -= 0.1f;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_LEFT))
		{
			camera_rotation[1] += 0.1f;
		}
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