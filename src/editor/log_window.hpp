#pragma once


#include <imgui.h>


//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
//-----------------------------------------------------------------------------

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
class ExampleAppLog {

public:
    ExampleAppLog();

    void    Clear();
    void    AddLog(const char* fmt, ...) IM_FMTARGS(2);    
    void    Draw(const char* title, bool* p_open = NULL);

private:
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.
};

// Demonstrate creating a simple log window with basic filtering.
static void ShowExampleAppLog(bool* p_open);

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
//-----------------------------------------------------------------------------

// Demonstrate create a window with multiple child windows.
static void ShowExampleAppLayout(bool* p_open);