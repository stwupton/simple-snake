workspace 'SDL Playground'
  configurations { 'Debug', 'Release' }
  platforms { 'Win64' }
  location 'build'
  
project 'sdl-playground'
  kind 'WindowedApp'
  language 'C++'
  cppdialect 'C++17'
  files { 'src/main.cpp' }
  includedirs { 'src', 'include' }
  libdirs { 'lib', os.findlib('SDL2') }
  
  filter 'configurations:Release'
    defines { 'NDEBUG' }
    optimize 'On'
  
  filter 'configurations:Debug'
    symbols 'On'

  filter 'platforms:Win64'
    system 'Windows'
    architecture 'x86_64'
  
  filter { 'system:Windows', 'toolset:clang' }
    linkoptions '-Xlinker /subsystem:windows'
  
  filter { 'configurations:Debug', 'system:Windows', 'toolset:clang' }
    linkoptions '-g'
  
  filter 'system:Windows'
    links { 'shell32', 'SDL2main', 'SDL2' }
    defines { 'WIN32', 'UNICODE' }