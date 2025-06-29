const std = @import("std");

const modules = @import("modules.zig");


pub fn reload(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Build Engine Library
    // ------------------------------------------------------------
    const engine = b.addLibrary(.{
        .linkage = .dynamic,
        .name = "physics",
        .root_module = modules.generateEngineModule(
            b, target, optimize
        )
    });    
    engine.installHeadersDirectory(
        b.path("src/physics"), "physics",
        .{.include_extensions = &.{"hpp", "h"}}
    );

    b.installArtifact(engine);
}