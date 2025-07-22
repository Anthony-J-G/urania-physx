// stdlib imports
const std = @import("std");
const Module = std.Build.Module;
const ResolvedTarget = std.Build.ResolvedTarget;
const OptimizeMode = std.builtin.OptimizeMode;

// config imports
const reload = @import("config/reload.zig");
const modules = @import("config/modules.zig");
const options = @import("config/options.zig");
const Options = options.Options;
const tests = @import("config/tests.zig");

// ccpputilz imports
const ccpputilz = @import("ccpputilz");
const compiledb = ccpputilz.default.compiledb;


pub fn build(b: *std.Build) void {
    const build_options = options.default(b);

    if (build_options.engine_is_packaged) {
        reload.reload(b, build_options);
        return;
    }

    // Build ImGui Static Library
    // ------------------------------------------------------------
    const libimgui = b.addLibrary(.{
        .linkage = .static,
        .name = "imgui",
        .root_module = modules.generateImGuiModule(b, build_options),
    });
    // ------------------------------------------------------------

    // Build Raylib Shared Library
    // ------------------------------------------------------------
    const raylib = b.dependency("raylib", .{ .shared = true });
    const libraylib = raylib.artifact("raylib");
    // ------------------------------------------------------------

    // Build Editor Library
    // ------------------------------------------------------------
    const editor = b.addLibrary(.{ .linkage = .static, .name = "editor", .root_module = modules.generateEditorModule(b, build_options) });
    editor.installHeadersDirectory(b.path("src/editor"), "editor", .{ .include_extensions = &.{ "hpp", "h" } });

    // ** Linking
    editor.linkLibrary(libimgui);
    editor.linkLibrary(libraylib);
    // ------------------------------------------------------------

    // Build Engine Library
    // ------------------------------------------------------------
    const engine = b.addLibrary(.{ .linkage = .dynamic, .name = "engine", .root_module = modules.generateEngineModule(b, build_options) });
    engine.installHeadersDirectory(b.path("src/engine"), "engine", .{ .include_extensions = &.{ "hpp", "h" } });

    // ** Linking
    engine.linkLibrary(libraylib);
    engine.linkLibrary(libimgui);
    // ------------------------------------------------------------

    // Build Runtime Executable
    // ------------------------------------------------------------
    const exe = b.addExecutable(.{ .name = "runtime", .root_module = modules.generateRuntimeModule(b, build_options) });
    // exe.addCSourceFile(.{
    //     .file = b.path("src/runtime/main.cpp"),
    //     .language = .cpp,
    //     .flags = &.{}
    // });

    // ** Includes
    exe.installLibraryHeaders(libraylib);

    // ** Linking
    exe.linkLibrary(libimgui);
    exe.linkLibrary(libraylib);
    exe.linkLibrary(editor);
    // ------------------------------------------------------------

    b.installArtifact(libraylib);
    b.installArtifact(engine);
    b.installArtifact(exe);

    if (build_options.should_build_tests) {
        tests.build(b, build_options);
    }

    compiledb.generateFromInstallStep(b);
}
