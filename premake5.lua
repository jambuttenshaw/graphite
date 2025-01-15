workspace "sampleApp"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "sampleApp"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "graphite"
include "sampleApp"
