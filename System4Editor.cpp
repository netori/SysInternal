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

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct System4Editor::ExampleAppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    ExampleAppLog()
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
    if (show_constrained_window)
        System4Editor::ShowExampleAppConstrainedResize(&show_constrained_window);
    if (show_app_log)
        System4Editor::ShowExampleAppLog(&show_app_log);
    return System4Editor::show_main_window;
}

void System4Editor::ShowMainWindow()
{
    // Create a window called "My First Tool", with a menu bar.
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("System 4 Runtime Editor", &show_main_window, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Constrained Resize")) { show_constrained_window = true; }
            if (ImGui::MenuItem("App Log")) { show_app_log = true; }
			ImGui::EndMenu();
		}
        ImGui::EndMenuBar();
    }

    // Edit a color stored as 4 floats
    //ImGui::ColorEdit4("Color", 1);

    // Generate samples and plot them
    float samples[100];
    for (int n = 0; n < 100; n++)
        samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    ImGui::Text("Module Address: %08X",Interface::moduleBase);
    ImGui::Text("CJaffaVM: %08X",Interface::GetCJaffaVM());
    ImGui::EndChild();
    ImGui::End();
}

// 
void System4Editor::ShowExampleAppConstrainedResize(bool* p_open)
{
    struct CustomConstraints
    {
        // Helper functions to demonstrate programmatic constraints
        // FIXME: This doesn't take account of decoration size (e.g. title bar), library should make this easier.
        static void AspectRatio(ImGuiSizeCallbackData* data) { float aspect_ratio = *(float*)data->UserData; data->DesiredSize.x = IM_MAX(data->CurrentSize.x, data->CurrentSize.y); data->DesiredSize.y = (float)(int)(data->DesiredSize.x / aspect_ratio); }
        static void Square(ImGuiSizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = IM_MAX(data->CurrentSize.x, data->CurrentSize.y); }
        static void Step(ImGuiSizeCallbackData* data) { float step = *(float*)data->UserData; data->DesiredSize = ImVec2((int)(data->CurrentSize.x / step + 0.5f) * step, (int)(data->CurrentSize.y / step + 0.5f) * step); }
    };

    const char* test_desc[] =
    {
        "Between 100x100 and 500x500",
        "At least 100x100",
        "Resize vertical only",
        "Resize horizontal only",
        "Width Between 400 and 500",
        "Custom: Aspect Ratio 16:9",
        "Custom: Always Square",
        "Custom: Fixed Steps (100)",
    };

    // Options
    static bool auto_resize = false;
    static bool window_padding = true;
    static int type = 5; // Aspect Ratio
    static int display_lines = 10;

    // Submit constraint
    float aspect_ratio = 16.0f / 9.0f;
    float fixed_step = 100.0f;
    if (type == 0) ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(500, 500));         // Between 100x100 and 500x500
    if (type == 1) ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
    if (type == 2) ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0), ImVec2(-1, FLT_MAX));      // Vertical only
    if (type == 3) ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));      // Horizontal only
    if (type == 4) ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(500, -1));          // Width Between and 400 and 500
    if (type == 5) ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::AspectRatio, (void*)&aspect_ratio);   // Aspect ratio
    if (type == 6) ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);                              // Always Square
    if (type == 7) ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Step, (void*)&fixed_step);            // Fixed Step

    // Submit window
    if (!window_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    const ImGuiWindowFlags window_flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
    const bool window_open = ImGui::Begin("Example: Constrained Resize", p_open, window_flags);
    if (!window_padding)
        ImGui::PopStyleVar();
    if (window_open)
    {
        if (ImGui::GetIO().KeyShift)
        {
            // Display a dummy viewport (in your real app you would likely use ImageButton() to display a texture.
            ImVec2 avail_size = ImGui::GetContentRegionAvail();
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::ColorButton("viewport", ImVec4(0.5f, 0.2f, 0.5f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, avail_size);
            ImGui::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y + 10));
            ImGui::Text("%.2f x %.2f", avail_size.x, avail_size.y);
        }
        else
        {
            ImGui::Text("(Hold SHIFT to display a dummy viewport)");
            if (ImGui::IsWindowDocked())
                ImGui::Text("Warning: Sizing Constraints won't work if the window is docked!");
            if (ImGui::Button("Set 200x200")) { ImGui::SetWindowSize(ImVec2(200, 200)); } ImGui::SameLine();
            if (ImGui::Button("Set 500x500")) { ImGui::SetWindowSize(ImVec2(500, 500)); } ImGui::SameLine();
            if (ImGui::Button("Set 800x200")) { ImGui::SetWindowSize(ImVec2(800, 200)); }
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 20);
            ImGui::Combo("Constraint", &type, test_desc, IM_ARRAYSIZE(test_desc));
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 20);
            ImGui::DragInt("Lines", &display_lines, 0.2f, 1, 100);
            ImGui::Checkbox("Auto-resize", &auto_resize);
            ImGui::Checkbox("Window padding", &window_padding);
            for (int i = 0; i < display_lines; i++)
                ImGui::Text("%*sHello, sailor! Making this line long enough for the example.", i * 4, "");
        }
    }
    ImGui::End();
}

// Demonstrate creating a simple log window with basic filtering.
void System4Editor::ShowExampleAppLog(bool* p_open)
{
    static System4Editor::ExampleAppLog log;

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Virtual Machine Logger", p_open);
    if (ImGui::SmallButton("Dump VM State"))
    {
        const uintptr_t VM_IP = Data::Read<uintptr_t>(0x062874E8+0x224);
        const uint16_t Bytecode = Data::Read<uint16_t>(VM_IP);
        const uint32_t OptionalArg = Data::Read<uint32_t>(VM_IP+0x2);

        log.AddLog("VM_IP:        %08X\nBytecode:     %04X\nOptional Arg: %08X\nVM_TOPMOST:   %08X\nVM_RETURN_TO: %08X\n/**/\n"
            , VM_IP, Bytecode, OptionalArg, 100, 69);
    }
    ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    log.Draw("Virtual Machine Logger", p_open);
}