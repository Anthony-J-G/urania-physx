// stdlib imports
const std               = @import("std");
const Module            = std.Build.Module;
const Compile           = std.Build.Step.Compile;
const ResolvedTarget    = std.Build.ResolvedTarget;
const OptimizeMode      = std.builtin.OptimizeMode;

// buildsystem imports
const Options = @import("../build.zig").Options;


pub fn build(b: *std.Build, opts: Options) *Compile {    
    // ** Create Module
    const module = b.createModule(.{
        .target = opts.target,
        .optimize = opts.optimize,
        .link_libc = true,
        .link_libcpp = true,
    });
    // ** Module: Add Sources
    module.addCSourceFiles(.{
        .root = b.path("src/engine"),
        .language = .c,
        .flags = &.{"-std=c89"},
        .files = engine_files,
    });
    module.addIncludePath(b.path("src"));    

    // ** Create Compile
    const lib = b.addLibrary(.{
        .linkage = if (opts.shared)
            .dynamic
        else
            .static
        ,
        .name = "urania",
        .root_module = module,
    });

    return lib;
}


pub const engine_files = &[_][]const u8{
    // Root Source Files
    "world.c",
};