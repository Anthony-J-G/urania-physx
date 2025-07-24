// stdlib imports
const std               = @import("std");
const Module            = std.Build.Module;
const Compile                 = std.Build.Step.Compile;
const ResolvedTarget    = std.Build.ResolvedTarget;
const OptimizeMode      = std.builtin.OptimizeMode;

// buildsystem imports
const Options = @import("../build.zig").Options;


pub fn build(b: *std.Build, opts: Options) *Compile {
    // ** Dependencies
    const rl_imgui = b.lazyDependency("rlimgui", .{}) orelse @panic("lazy dependecy error");
    const imgui = b.lazyDependency("imgui", .{})  orelse @panic("lazy dependecy error");
    const raylib = b.lazyDependency("raylib", .{.target = opts.target, .optimize = opts.optimize}) orelse @panic("lazy dependecy error");
    const json = b.lazyDependency("json", .{}) orelse @panic("lazy dependecy error");

    // ** Create Module
    const module = b.createModule(.{
        .target = opts.target,
        .optimize = opts.optimize,
        .link_libc = true,
        .link_libcpp = true,
    });
    // ** Module: Add Sources
    module.addCSourceFiles(.{
        .root = b.path("src/examples"),
        .language = .cpp,
        .flags = &.{},
        .files = examples_files,
    });
    module.addCSourceFiles(.{
        .root = imgui.path(""),
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "imgui.cpp",
            "imgui_demo.cpp",
            "imgui_draw.cpp",
            "imgui_tables.cpp",
            "imgui_widgets.cpp",
        },
    });
    module.addCSourceFiles(.{
        .root = rl_imgui.path(""),
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "rlImGui.cpp",
        },
    });

    module.addIncludePath(b.path("src"));
    module.addIncludePath(rl_imgui.path(""));
    module.addIncludePath(imgui.path(""));
    module.addIncludePath(raylib.path("src"));
    module.addIncludePath(json.path("include"));

    // Create Compile
    const exe = b.addExecutable(.{
        .name = "examples",
        .root_module = module,
    });

    switch (opts.target.result.os.tag) {
        .windows => {
            // TODO(anthony-j-g): figure out how to use winucrt to track memory leaks
            // exe.linkSystemLibrary("ucrt");
        },

        else => {},

    }

    // Compile: Link Libraries
    exe.linkLibrary(raylib.artifact("raylib"));    

    return exe;
}


pub const examples_files = &[_][]const u8{
    // Root Source Files
    "main.cpp",

    // Runtime Source Files
    "runtime/app.cpp",
    "runtime/settings.cpp",
    "runtime/render.cpp",
    
    // Editor Source Files
    "editor/editor.cpp",
    "editor/scene_view_window.cpp",
    "editor/scene_list_window.cpp",
    "editor/control_window.cpp",

    // Scene Source Files
    "scenes/scene.cpp",
    "scenes/scene_gravity.cpp",
};