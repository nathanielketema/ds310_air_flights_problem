const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "air_zig",
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFiles(.{
        .files = &.{
            "air.cpp",
            "Set.cpp",
        },
    });
    exe.linkLibCpp();

    b.installArtifact(exe);

    const run_step = b.step("run",
        \\Run program with default input files:
        \\                                   - listOfAirports.txt
        \\                                   - sampleFlights.txt
        \\ Note: the order of input files matters!
    );
    const run_exe = b.addRunArtifact(exe);

    if (b.args) |args| {
        run_exe.addArgs(args);
    } else {
        const default_args = &.{
            "listOfAirports.txt",
            "sampleFlights.txt",
        };
        run_exe.addArgs(default_args);
    }

    run_step.dependOn(&run_exe.step);
}
