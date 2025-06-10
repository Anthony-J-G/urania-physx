const std = @import("std");

const ccpputilz = @import("ccpputilz");
const compile_commands = ccpputilz.compile_commands;


pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const raylib = b.dependency("raylib", .{.shared=true});
    const libraylib = raylib.artifact("raylib");
    b.installArtifact(libraylib);

    const lib = b.addLibrary(.{
        .linkage = .static,
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
            "sample.cpp",
            "fluid_sim.cpp",
        },
    });

    // ** Includes
    lib.addIncludePath(b.path("src/physics"));
    lib.addIncludePath(b.path("zig-out/include"));
    lib.installLibraryHeaders(libraylib);
    lib.installHeadersDirectory(b.path("src/physics"), "physics", .{.include_extensions = &.{"hpp", "h"}});

    // ** Linking
    lib.linkLibrary(libraylib);

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
            "main.cpp"
        },
    });
    // ** Includes
    exe.installLibraryHeaders(libraylib);
    exe.addIncludePath(b.path("zig-out/include"));

    // ** Linking
    exe.linkLibrary(lib);
    exe.linkLibrary(libraylib);

    b.installArtifact(exe);

    try compile_commands.generateCompileCommands(b, &.{
        lib,
        exe,
    });
}
