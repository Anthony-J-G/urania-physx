// stdlib imports
const std = @import("std");
const Module = std.Build.Module;
const ResolvedTarget = std.Build.ResolvedTarget;
const OptimizeMode = std.builtin.OptimizeMode;

// config imports
const engine    = @import("src/build-engine.zig");
const examples  = @import("src/build-examples.zig");

// ccpputilz imports
const ccpputilz = @import("ccpputilz");
const compiledb = ccpputilz.default.compiledb;


pub const Options = struct {
    target: ResolvedTarget,
    optimize: OptimizeMode,

    engine_is_packaged: bool = false,
    should_build_tests: bool = true,
};


pub fn default_options(b: *std.Build) Options {
    return .{
        .target = b.standardTargetOptions(.{}),
        .optimize = b.standardOptimizeOption(.{}),
        .engine_is_packaged = b.option(bool, "recompile-from-engine", "for internal use by the engine only") orelse false,
        .should_build_tests = b.option(bool, "tests", "Compile tests and add test command") orelse true,
    };
}


pub fn build(b: *std.Build) void {
    const opts = default_options(b);

    const engine_lib = engine.build(b, opts);
    b.installArtifact(engine_lib);

    const examples_exe = examples.build(b, opts);
    examples_exe.linkLibrary(engine_lib);
    b.installArtifact(examples_exe);

    compiledb.generateFromInstallStep(b);
}
