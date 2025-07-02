const std = @import("std");
const Module = std.Build.Module;
const ResolvedTarget = std.Build.ResolvedTarget;
const OptimizeMode = std.builtin.OptimizeMode;



pub const Options = struct {
    target: ResolvedTarget,
    optimize: OptimizeMode,

    engine_is_packaged: bool = false,
    should_build_tests: bool = true,

};


pub fn default(b: *std.Build) Options {
    return .{
        .target = b.standardTargetOptions(.{}),
        .optimize = b.standardOptimizeOption(.{}),
        .engine_is_packaged = b.option(bool, "recompile-from-engine", "for internal use by the engine only") orelse false,
        .should_build_tests = b.option(bool, "tests", "Compile tests and add test command") orelse true,
    };
}