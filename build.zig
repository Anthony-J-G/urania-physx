const std = @import("std");
const Module = std.Build.Module;
const ResolvedTarget = std.Build.ResolvedTarget;
const OptimizeMode = std.builtin.OptimizeMode;

const ccpputilz = @import("ccpputilz");
const compile_commands = ccpputilz.compile_commands;

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Build ImGui Static Library
    // ------------------------------------------------------------
    const libimgui = b.addLibrary(.{
        .linkage = .static,
        .name = "imgui",
        .root_module = generateImGuiModule(
            b, target, optimize
        ),
    });
    // ------------------------------------------------------------

    // Build Raylib Shared Library
    // ------------------------------------------------------------
    const raylib = b.dependency("raylib", .{.shared=true});
    const libraylib = raylib.artifact("raylib");
    // ------------------------------------------------------------

    // Build Engine Library
    // ------------------------------------------------------------
    const engine = b.addLibrary(.{
        .linkage = .dynamic,
        .name = "physics",
        .root_module = generateEngineModule(
            b, target, optimize
        )
    });    
    engine.installHeadersDirectory(
        b.path("src/physics"), "physics",
        .{.include_extensions = &.{"hpp", "h"}}
    );

    // ** Linking
    engine.linkLibrary(libraylib);
    engine.linkLibrary(libimgui);    
    // ------------------------------------------------------------

    // Build Runtime Executable
    // ------------------------------------------------------------
    const exe = b.addExecutable(.{
        .name = "runtime",
        .root_module = generateRuntimeModule(
            b, target, optimize
        )
    });

    // ** Includes
    exe.installLibraryHeaders(libraylib);    

    // ** Linking
    exe.linkLibrary(libimgui);
    exe.linkLibrary(libraylib);
    // ------------------------------------------------------------

    b.installArtifact(libraylib);
    b.installArtifact(engine);
    b.installArtifact(exe);

    try compile_commands.generateCompileCommands(b, &.{
        engine,
        exe,
    });
}


fn generateEditorModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
    const module = b.createModule(.{
        .target = t,
        .optimize = o,
        .link_libc = true,
        .link_libcpp = false,
    });

    return module;
}


fn generateEngineModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
    const module_directory = b.path("src/physics/");

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
        },
    });    

    // ** Includes
    module.addIncludePath(b.path("src"));
    module.addIncludePath(module_directory);
    module.addIncludePath(b.path("zig-out/include"));
    return module;
}


fn generateRuntimeModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
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
            "editor/editor.cpp",
            "editor/image_viewer_window.cpp",
            "editor/scene_list_window.cpp",
            "editor/scene_view_window.cpp",
        },
    });
    if (t.result.os.tag == .windows) {
        module.addCSourceFiles(.{
            .root = b.path("src/runtime"),
            .language = .cpp,
            .flags = &.{},
            .files = &.{
                "win32_dynamic_api.cpp",
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


fn generateImGuiModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
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
    module.addIncludePath(imgui.path(""));

    return module;    
}


fn generateCommonModule(b: *std.Build, t: ResolvedTarget, o: OptimizeMode) *Module {
    const module = b.createModule(.{
        .target = t,
        .optimize = o,
        .link_libc = true,
        .link_libcpp = false,
    });
    return module;
}