//!

const std = @import("std");

const modules = @import("modules.zig");
const Options = @import("options.zig").Options;

pub fn reload(b: *std.Build, opts: Options) void {

    // Build Engine Library
    // ------------------------------------------------------------
    const engine = b.addLibrary(.{
        .linkage = .dynamic,
        .name = "physics",
        .root_module = modules.generateEngineModule(
            b, opts
        )
    });    
    engine.installHeadersDirectory(
        b.path("src/physics"), "physics",
        .{.include_extensions = &.{"hpp", "h"}}
    );

    b.installArtifact(engine);
}