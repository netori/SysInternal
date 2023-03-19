#pragma once
#include <math.h>
#include "imgui.h"
#include "imgui_memory_editor.h"
#include "Interface.h"

namespace System4Editor
{
	static bool isSetup = false;
	static bool isOpen = true;
	static bool my_tool_active = true;
	static bool show_system4_editor = true;
	static bool isMemoryEditorOpen = false;
	bool Show();
	void Render();
	void Setup();
	
};