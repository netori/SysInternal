#include <chrono>
#include <ctime>  
#include "System4Editor.h"
#include "Data.h"

// Helpers macros
// We normally try to not use many helpers in imgui_demo.cpp in order to make code easier to copy and paste,
// but making an exception here as those are largely simplifying code...
// In other imgui sources we can use nicer internal functions from imgui_internal.h (ImMin/ImMax) but not in the demo.
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
//-----------------------------------------------------------------------------

struct System4Editor::SystemAppLog
{
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool                AutoScroll;  // Keep scrolling if already at the bottom.

	SystemAppLog()
	{
		AutoScroll = true;
		Clear();
	}

	void    Clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
	}

	void    Draw(const char* title, bool* p_open = NULL)
	{
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		// Main window
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);

		ImGui::Separator();

		if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			if (clear)
				Clear();
			if (copy)
				ImGui::LogToClipboard();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			const char* buf = Buf.begin();
			const char* buf_end = Buf.end();
			if (Filter.IsActive())
			{
				// In this example we don't use the clipper when Filter is enabled.
				// This is because we don't have random access to the result of our filter.
				// A real application processing logs with ten of thousands of entries may want to store the result of
				// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
				for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					if (Filter.PassFilter(line_start, line_end))
						ImGui::TextUnformatted(line_start, line_end);
				}
			}
			else
			{
				// The simplest and easy way to display the entire buffer:
				//   ImGui::TextUnformatted(buf_begin, buf_end);
				// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
				// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
				// within the visible area.
				// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
				// on your side is recommended. Using ImGuiListClipper requires
				// - A) random access into your data
				// - B) items all being the  same height,
				// both of which we can handle since we have an array pointing to the beginning of each line of text.
				// When using the filter (in the block of code above) we don't have random access into the data to display
				// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
				// it possible (and would be recommended if you want to search through tens of thousands of entries).
				ImGuiListClipper clipper;
				clipper.Begin(LineOffsets.Size);
				while (clipper.Step())
				{
					for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
					{
						const char* line_start = buf + LineOffsets[line_no];
						const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
						ImGui::TextUnformatted(line_start, line_end);
					}
				}
				clipper.End();
			}
			ImGui::PopStyleVar();

			// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
			// Using a scrollbar or mouse-wheel will take away from the bottom edge.
			if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
		}
		ImGui::EndChild();
		ImGui::End();
	}
};

bool System4Editor::Render()
{
	System4Editor::ShowMainWindow();
	if (show_app_log)
		System4Editor::ShowAppLog(&show_app_log);
	return System4Editor::show_main_window;
}

void System4Editor::ShowMainWindow()
{
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("System 4 Runtime Editor", &show_main_window, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Text("ABOUT SYSINTERNAL");
			ImGui::BulletText("This tool was designed for Rance IX 1.00.");
			ImGui::Separator();

			ImGui::Text("USER GUIDE:");
			ImGui::ShowUserGuide();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("App Log")) { show_app_log = true; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader("System Virtual Machine Information"))
	{
		ImGui::Text("Module Address: %08X", sys43vm::hMod);
		ImGui::Text("CJaffaVM: %08X", Interface::GetCJaffaVMAdress());
	}

	if (ImGui::CollapsingHeader("Character Editor"))
	{
		ImGui::SeparatorText("Selected Character");
		const char* items[] = { "Rance", "Kanami Kentou", "Shizuka Masou", "Maria Custard", "Patton Misnarge", "Hubert Lipton", "Russian Cullet", "Maitreia Meicyan", "Rick Addison", "Tilde Sharp", "Crook Mofus", "Alkanese Rize", "Pitten Ciao", "Senhime", "Miracle Tou", "Sanada Tourin", "Pigu Geliciam", "Freya Idun", "Rolex Gadras", "Oruore the 3rd", "Hunty Kalar"};
		static int item_current = 0;
		ImGui::Combo("Character", &item_current, items, IM_ARRAYSIZE(items));
		// Get the respective CPage
		sys43vm::Character* chara = Interface::GetCharacter(item_current);
		
		ImGui::SeparatorText("Information");
		ImGui::Text("nID: %d", chara->nID);
		ImGui::Text("CPage: %08X", &chara);

		constexpr ImU32 one_step = 1;
		ImGui::SeparatorText("Statistics");
		ImGui::InputScalar("Cur EVD", ImGuiDataType_U32, &chara->Cur_EVD, &one_step, NULL, "%u");
		ImGui::InputScalar("Max EVD", ImGuiDataType_U32, &chara->Max_EVD, &one_step, NULL, "%u");
		ImGui::InputScalar("Cur PRY", ImGuiDataType_U32, &chara->Cur_PRY, &one_step, NULL, "%u");
		ImGui::InputScalar("Max PRY", ImGuiDataType_U32, &chara->Max_PRY, &one_step, NULL, "%u");
		ImGui::InputScalar("Cur AMR", ImGuiDataType_U32, &chara->Cur_AMR, &one_step, NULL, "%u");
		ImGui::InputScalar("Max AMR", ImGuiDataType_U32, &chara->Max_AMR, &one_step, NULL, "%u");
		ImGui::InputScalar("Cur HP", ImGuiDataType_U32, &chara->Cur_HP, &one_step, NULL, "%u");
		ImGui::InputScalar("Max HP", ImGuiDataType_U32, &chara->Max_HP, &one_step, NULL, "%u");
		ImGui::InputScalar("Cur SP", ImGuiDataType_U32, &chara->Cur_SP, &one_step, NULL, "%u");
		ImGui::InputScalar("Max SP", ImGuiDataType_U32, &chara->Max_SP, &one_step, NULL, "%u");
		ImGui::InputScalar("Cur TCT", ImGuiDataType_U32, &chara->Cur_TCT, &one_step, NULL, "%u");
		ImGui::InputScalar("Max TCT", ImGuiDataType_U32, &chara->Max_TCT, &one_step, NULL, "%u");
		ImGui::InputScalar("Min ATK", ImGuiDataType_U32, &chara->Min_ATK, &one_step, NULL, "%u");
		ImGui::InputScalar("Max ATK", ImGuiDataType_U32, &chara->Max_ATK, &one_step, NULL, "%u");
		ImGui::InputScalar("Abs MAT", ImGuiDataType_U32, &chara->Abs_ATK, &one_step, NULL, "%u");
		ImGui::InputScalar("Resist", ImGuiDataType_U32, &chara->Resist, &one_step, NULL, "%u");
		ImGui::InputScalar("Move", ImGuiDataType_U32, &chara->Move, &one_step, NULL, "%u");
		ImGui::InputScalar("Range", ImGuiDataType_U32, &chara->Range, &one_step, NULL, "%u");
	}

	ImGui::End();
}

// Demonstrate creating a simple log window with basic filtering.
void System4Editor::ShowAppLog(bool* p_open)
{
	static System4Editor::SystemAppLog log;

	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Virtual Machine Logger", p_open);
	if (ImGui::SmallButton("Dump VM State"))
	{
		const uintptr_t CJaffaVM = Interface::GetCJaffaVMAdress();
		// todo: change hardcoded offsets to something more dynamic
		const uintptr_t VM_IP = Data::Read<uintptr_t>(CJaffaVM + 0x224);
		const uint16_t Bytecode = Data::Read<uint16_t>(VM_IP);
		const uint32_t OptionalArg = Data::Read<uint32_t>(VM_IP + 0x2);

		std::string fmstring = "VM_IP:        %08X\nBytecode:     %04X\nOptional Arg: %08X\nVM_TOPMOST:   %08X\nVM_RETURN_TO: %08X\n/**/\n";

		log.AddLog("VM_IP:        %08X\nBytecode:     %04X\nOptional Arg: %08X\nVM_TOPMOST:   %08X\nVM_RETURN_TO: %08X\n/**/\n"
			, VM_IP, Bytecode, OptionalArg, 100, 69);
	}
	ImGui::End();

	// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
	log.Draw("Virtual Machine Logger", p_open);
}