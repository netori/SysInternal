#pragma once
#include <math.h>
#include "imgui.h"
#include "imgui_memory_editor.h"
#include "Interface.h"
#include "System4VM.h"

namespace System4Editor
{
	struct ExampleAppLog;
	static bool isOpen = true;

	static bool show_main_window = true;
	static bool show_constrained_window = false;
	static bool show_app_log = false;
	static bool show_app_about = false;

	bool Render();
	void ShowMainWindow();
	void ShowExampleAppConstrainedResize(bool*);
	void ShowExampleAppLog(bool*);

};