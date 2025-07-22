// stdlib imports
const std               = @import("std");
const Module            = std.Build.Module;
const ResolvedTarget    = std.Build.ResolvedTarget;
const OptimizeMode      = std.builtin.OptimizeMode;

// buildsystem imports
const Options = @import("options.zig").Options;



pub fn generateEditorModule(b: *std.Build, opts: Options) *Module {
    // ** Dependencies
    const rl_imgui = b.dependency("rlimgui", .{});
    const imgui = b.dependency("imgui", .{});

    const module = b.createModule(.{
        .target = opts.target,
        .optimize = opts.optimize,
        .link_libc = true,
        .link_libcpp = true,
    });
    module.addCSourceFiles(.{
        .root = b.path("src/editor"),
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "editor.cpp",
            "image_viewer_window.cpp",
            "scene_list_window.cpp",
            "scene_view_window.cpp",
            "compiler_window.cpp"
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
    
    return module;
}


pub fn generateEngineModule(b: *std.Build, opts: Options) *Module {
    const module_directory = b.path("src/engine/");

    const module = b.createModule(.{
        .target = opts.target,
        .optimize = opts.optimize,
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
    module.addCMacro("ENGINE_LIBRARY_EXPORTS", "");

    // ** Includes
    module.addIncludePath(b.path("src"));
    module.addIncludePath(module_directory);
    module.addIncludePath(b.path("zig-out/include"));
    return module;
}


pub fn generateRuntimeModule(b: *std.Build, opts: Options) *Module {
    const rl_imgui = b.dependency("rlimgui", .{});
    const imgui = b.dependency("imgui", .{});

    const module = b.createModule(.{
        .root_source_file = b.path("src/runtime/main.zig"),
        .target = opts.target,
        .optimize = opts.optimize,
        .link_libc = true,
        .link_libcpp = true,
    });
    // ** Sources
    module.addCSourceFiles(.{
        .root = b.path("src/runtime"),
        .language = .cpp,
        .flags = &.{},
        .files = &.{            
            "backend.cpp",
        },
    });
    // if (opts.target.result.os.tag == .windows) {
    //     module.addCSourceFiles(.{
    //         .root = b.path("src/runtime"),
    //         .language = .cpp,
    //         .flags = &.{},
    //         .files = &.{
    //             "dynlib/win32_load.cpp",
    //         },
    //     });
    // }    

    // ** Include Paths
    module.addIncludePath(b.path("src"));
    module.addIncludePath(b.path("src/runtime/"));    
    module.addIncludePath(b.path("zig-out/include"));
    module.addIncludePath(imgui.path(""));
    module.addIncludePath(rl_imgui.path(""));

    return module;
}


pub fn generateImGuiModule(b: *std.Build, opts: Options) *Module {
    // ** Dependencies
    const imgui = b.dependency("imgui", .{});

    const module = b.createModule(.{
        .target = opts.target,
        .optimize = opts.optimize,
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


pub fn generateCommonModule(b: *std.Build, opts: Options) *Module {
    const module = b.createModule(.{
        .target = opts.target,
        .optimize = opts.optimize,
        .link_libc = true,
        .link_libcpp = false,
    });
    return module;
}