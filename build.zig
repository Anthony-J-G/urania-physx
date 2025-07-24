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

    engine: bool = false,
    shared: bool = false,
    examples: bool = false,
    headers_only: bool = false,
};

pub fn default_options(b: *std.Build) Options {
    return .{
        .target = b.standardTargetOptions(.{}),
        .optimize = b.standardOptimizeOption(.{}),
        .engine = b.option(bool, "engine", "Only compile the engine binary") orelse false,
        .shared = b.option(bool, "shared", "Compile the engine binary as a shared object (dynamic library)") orelse false,
        .examples = b.option(bool, "examples", "Only compile the examples executable") orelse false,
        .headers_only = b.option(bool, "headers-only", "Only install the engine's header files") orelse false,
    };
}


pub fn build(b: *std.Build) void {
    const opts = default_options(b);
    
    const install_engine = (
        opts.engine or (!opts.examples and !opts.engine)
    ) and !opts.headers_only;

    const install_examples = (
        opts.examples or (!opts.examples and !opts.engine)
    ) and !opts.headers_only;

    // Installation
    // -------------------------------
    
    const engine_lib = engine.build(b, opts);
    if (install_engine) {
        b.installArtifact(engine_lib);    
    }
    
    if (install_examples) {
        const examples_exe = examples.build(b, opts);
        examples_exe.linkLibrary(engine_lib);

        b.installArtifact(examples_exe);
    }
    // ------------------------------- end installation  

    compiledb.generateFromInstallStep(b);
}
