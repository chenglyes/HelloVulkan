add_rules("mode.debug", "mode.release")


set_languages("cxx20")


add_requires("stb", "glm", "glfw", "vulkansdk")
add_packages("stb", "glm", "glfw", "vulkansdk")


set_targetdir("Bin")
set_rundir(".")


target("HelloVulkan")
    set_kind("binary")

    add_includedirs("HelloVulkan")
    add_headerfiles("HelloVulkan/**.h")
    add_files("HelloVulkan/**.cpp")
