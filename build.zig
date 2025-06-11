const std = @import("std");

const ccpputilz = @import("ccpputilz");
const compile_commands = ccpputilz.compile_commands;


pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Build ImGui Static Library
    // ------------------------------------------------------------
    const imgui = b.dependency("imgui", .{});
    const libimgui = b.addLibrary(.{
        .linkage = .static,
        .name = "imgui",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
            .link_libcpp = true,
        }),
    });
    libimgui.addCSourceFiles(.{
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
    libimgui.installHeadersDirectory(imgui.path(""), "", .{.include_extensions = &.{"hpp", "h"}});
    // ------------------------------------------------------------

    const raylib = b.dependency("raylib", .{.shared=true});
    const libraylib = raylib.artifact("raylib");
    b.installArtifact(libraylib);

    const rl_imgui = b.dependency("rlimgui", .{});

    const lib = b.addLibrary(.{
        .linkage = .dynamic,
        .name = "physics",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
            .link_libcpp = true,
        }),
    });
    // ** Sources
    lib.addCSourceFiles(.{
        .root = b.path("src/physics"),
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "engine.cpp",
            "scenes/sample.cpp",
            "scenes/fluid_sim.cpp",
        },
    });    

    // ** Includes
    lib.addIncludePath(b.path("src/physics"));
    lib.addIncludePath(b.path("zig-out/include"));
    lib.installLibraryHeaders(libraylib);
    lib.installHeadersDirectory(b.path("src/physics"), "physics", .{.include_extensions = &.{"hpp", "h"}});

    // ** Linking
    lib.linkLibrary(libraylib);
    lib.linkLibrary(libimgui);

    b.installArtifact(lib);

    const exe = b.addExecutable(.{
        .name = "runtime",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });
    // ** Sources
    exe.addCSourceFiles(.{
        .root = b.path("src/runtime"),
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "main.cpp",
            "editor.cpp"
        },
    });
    if (target.result.os.tag == .windows) {
        exe.addCSourceFiles(.{
            .root = b.path("src/runtime"),
            .language = .cpp,
            .flags = &.{},
            .files = &.{
                "win32_dynamic_api.cpp",
            },
        });
    }
    exe.addCSourceFiles(.{
        .root = rl_imgui.path(""),
        .language = .cpp,
        .flags = &.{},
        .files = &.{
            "rlImGui.cpp",
        },
    });

    // ** Includes
    exe.installLibraryHeaders(libraylib);
    exe.addIncludePath(b.path("zig-out/include"));
    exe.addIncludePath(imgui.path(""));
    exe.addIncludePath(rl_imgui.path(""));

    // ** Linking
    exe.linkLibrary(libimgui);
    exe.linkLibrary(libraylib);

    b.installArtifact(exe);

    try compile_commands.generateCompileCommands(b, &.{
        lib,
        exe,
    });
}
