//!

// stdlib imports
const std = @import("std");
const ResolvedTarget = std.Build.ResolvedTarget;
const OptimizeMode = std.builtin.OptimizeMode;
const Step = std.Build.Step;

// buildsystem imports
const modules = @import("modules.zig");
const Options = @import("options.zig").Options;

// ccpputilz imports
const ccpputilz = @import("ccpputilz");
const gtest = ccpputilz.extensions.gtest;
const discover = ccpputilz.default.discover;


pub fn build(b: *std.Build, opts: Options) void {
    const runtime_tests: *Step.Compile = gtest.createTestStep(b, .{
        .name = "runtime_tests",
        .target = opts.target,
        .optimize = opts.optimize,
    });
    discover.discoverCSourceFiles(runtime_tests, .{
        .root = b.path("tests/unit_tests/runtime"),
        .flags = &.{"-std=c++17"},
    });
    runtime_tests.root_module.addImport("runtime", modules.generateRuntimeModule(b, opts));
    runtime_tests.addIncludePath(b.path("src"));

    b.installArtifact(runtime_tests);
}
