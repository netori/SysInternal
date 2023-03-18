#include "System4Editor.h"

void System4Editor::Render()
{
	char buffer[100];
	float a = 0;
	ImGui::Text("Hello World.");
	if (ImGui::Button("Save"))
	{

	}
	ImGui::InputText("string", buffer, IM_ARRAYSIZE(buffer));
	ImGui::SliderFloat("float", &a, 0.0f, 1.05f);
}
