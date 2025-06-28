const std = @import("std");
const Module = std.Build.Module;
const ResolvedTarget = std.Build.ResolvedTarget;
const OptimizeMode = std.builtin.OptimizeMode;

const reload = @import("config/reload.zig").reload;
const modules = @import("config/modules.zig");

const ccpputilz = @import("ccpputilz");
const compile_commands = ccpputilz.compile_commands;

const tests = @import("tests/tests.zig");


pub fn build(b: *std.Build) void {
    if (
        b.option(bool, "recompile-from-engine", "for internal use by the engine only") orelse false
    ) {
        reload(b);
        return;
    }

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Build ImGui Static Library
    // ------------------------------------------------------------
    const libimgui = b.addLibrary(.{
        .linkage = .static,
        .name = "imgui",
        .root_module = modules.generateImGuiModule(
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
        .name = "engine",
        .root_module = modules.generateEngineModule(
            b, target, optimize
        )
    });    
    engine.installHeadersDirectory(
        b.path("src/engine"), "engine",
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
        .root_module = modules.generateRuntimeModule(
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