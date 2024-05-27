#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <TestGLFWCore/Input.hpp>
#include <TestGLFWCore/Application.hpp>


class TestGLFWEditor : public TestGLFW::Application
{
	void on_update() override
	{
		bool move_camera = false;
		glm::vec3 movement_delta{ 0.f, 0.f, 0.f };
		glm::vec3 rotation_delta{ 0.f, 0.f, 0.f };

		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_W))
		{
			movement_delta.x += 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_S))
		{
			movement_delta.x -= 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_A))
		{
			movement_delta.y -= 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_D))
		{
			movement_delta.y += 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_E))
		{
			movement_delta.z += 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_Q))
		{
			movement_delta.z -= 0.005f;
			move_camera = true;
		}

		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_UP))
		{
			rotation_delta.y -= 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_DOWN))
		{
			rotation_delta.y += 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_RIGHT))
		{
			rotation_delta.z -= 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_LEFT))
		{
			rotation_delta.z += 0.005f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_P))
		{
			rotation_delta.x += 0.05f;
			move_camera = true;
		}
		if (TestGLFW::Input::isKeyPressed(TestGLFW::KeyCode::KEY_O))
		{
			rotation_delta.x -= 0.05f;
			move_camera = true;
		}

		if (move_camera)
		{
			camera.add_movement_and_rotation(movement_delta, rotation_delta);
		}
	}


	void on_ui_draw() override
	{
		camera_position[0] = camera.get_camera_position().x;
		camera_position[1] = camera.get_camera_position().y;
		camera_position[2] = camera.get_camera_position().z;

		camera_rotation[0] = camera.get_camera_rotation().x;
		camera_rotation[1] = camera.get_camera_rotation().y;
		camera_rotation[2] = camera.get_camera_rotation().z;
		
		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("camera position", camera_position, -10.0f, 10.0f))
		{
			camera.set_position(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
		};
		if (ImGui::SliderFloat3("camera rotation", camera_rotation, 0.0f, 360.0f))
		{
			camera.set_rotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
		}
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