const std = @import("std");
const Module = std.Build.Module;
const ResolvedTarget = std.Build.ResolvedTarget;
const OptimizeMode = std.builtin.OptimizeMode;



pub fn generateEditorModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
    const module = b.createModule(.{
        .target = t,
        .optimize = o,
        .link_libc = true,
        .link_libcpp = false,
    });

    return module;
}


pub fn generateEngineModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
    const module_directory = b.path("src/engine/");

    const module = b.createModule(.{
        .target = t,
        .optimize = o,
        .link_libc = true,
        .link_libcpp = true,
    });
    // ** Sources
    module.addCSourceFiles(.{
        .root = module_directory,
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "engine.cpp",
            "scene.cpp",
            "scenes/sample.cpp",
            "scenes/fluid_sim.cpp",
            "scenes/euler_fluid_sim_2d.cpp",
        },
    });    

    // ** Includes
    module.addIncludePath(b.path("src"));
    module.addIncludePath(module_directory);
    module.addIncludePath(b.path("zig-out/include"));
    return module;
}


pub fn generateRuntimeModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
    // ** Dependencies
    const rl_imgui = b.dependency("rlimgui", .{});

    const module = b.createModule(.{
        .target = t,
        .optimize = o,
        .link_libc = true,
        .link_libcpp = true,
    });
    // ** Sources
    module.addCSourceFiles(.{
        .root = b.path("src/runtime"),
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "main.cpp",
            "dynlib/common_api.cpp",
            "editor/editor.cpp",
            "editor/image_viewer_window.cpp",
            "editor/scene_list_window.cpp",
            "editor/scene_view_window.cpp",
            "editor/compiler_window.cpp"
        },
    });
    if (t.result.os.tag == .windows) {
        module.addCSourceFiles(.{
            .root = b.path("src/runtime"),
            .language = .cpp,
            .flags = &.{},
            .files = &.{
                "dynlib/win32_load.cpp",
            },
        });
    }
    module.addCSourceFiles(.{
        .root = rl_imgui.path(""),
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "rlImGui.cpp",
        },
    });

    // ** Include Paths
    module.addIncludePath(b.path("src"));
    module.addIncludePath(b.path("src/runtime/"));
    module.addIncludePath(rl_imgui.path(""));    
    module.addIncludePath(b.path("zig-out/include"));

    return module;
}


pub fn generateImGuiModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
    // ** Dependencies
    const imgui = b.dependency("imgui", .{});

    const module = b.createModule(.{
        .target = t,
        .optimize = o,
        .link_libc = true,
        .link_libcpp = false,
    });

    // ** Sources
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

    // ** Include Paths
    module.addIncludePath(b.path("src"));
    module.addIncludePath(imgui.path(""));

    return module;    
}


pub fn generateCommonModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
    const module = b.createModule(.{
        .target = t,
        .optimize = o,
        .link_libc = true,
        .link_libcpp = false,
    });
    return module;
}