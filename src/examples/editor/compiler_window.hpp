#pragma once

#include "editor.hpp"
#include <imgui.h>



struct CompilerWindow : public EditorWindow {
    friend Editor;
public:
    CompilerWindow();
    CompilerWindow(const char* _title);
    ~CompilerWindow();
    
    void    Setup(Editor* editor_ref) override;
    void    Shutdown() override;    
    void    Draw() override;    
    void    Update() override;

    void    ClearLog();
    void    AddLog(const char* fmt, ...) IM_FMTARGS(2);    
    void    ExecCommand(const char* command_line);
    
    int     TextEditCallback(ImGuiInputTextCallbackData* data);

private:
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;
};