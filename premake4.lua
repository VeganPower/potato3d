solution "potato_ray"
   configurations { "Debug", "Release" }
   project "potato_ray"
      kind "ConsoleApp"
      language "C++"
      includedirs { "libs/glm" }
      links{ "glfw3", "GL" }
      --links{ "glfw3", "GL" }
      files { "./src/**.hpp", "./src/**.cpp" }

      --defines { "ISPC_USE_OMP" }
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols", "EnableSSE2" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "OptimizeSpeed", "ExtraWarnings", "EnableSSE2" }

      configuration { "linux", "gmake" }
         buildoptions { "-std=c++11" }
         linkoptions { "-fopenmp" }
         --linkoptions { "-fopenmp" }
         links{ "X11", "Xxf86vm", "Xi", "Xrandr", "Xinerama", "Xcursor"}
         --x11 xrandr xi xxf86vm gl
