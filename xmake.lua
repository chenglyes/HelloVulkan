add_rules("mode.debug", "mode.release")


set_languages("cxx20")


add_requires("stb", "tinyobjloader", "glm", "glfw", "vulkansdk")
add_packages("stb", "tinyobjloader", "glm", "glfw", "vulkansdk")


set_targetdir("Bin")
set_rundir(".")


if is_mode("debug") then
    set_suffixname("_d")
end


target("HelloVulkan")
    set_kind("binary")

    add_includedirs("HelloVulkan")
    add_headerfiles("HelloVulkan/**.h")
    add_files("HelloVulkan/**.cpp")
