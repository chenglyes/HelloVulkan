add_rules("mode.debug", "mode.release")


set_languages("cxx20")


add_requires("fmt", "glm", "glfw", "vulkansdk")
add_packages("fmt", "glm", "glfw", "vulkansdk")


set_targetdir("Bin")
set_rundir(".")


target("HelloVulkan")
    set_kind("binary")

    add_includedirs("HelloVulkan")
    add_headerfiles("HelloVulkan/**.h")
    add_files("HelloVulkan/**.cpp")
