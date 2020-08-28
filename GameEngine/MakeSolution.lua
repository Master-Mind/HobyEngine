json = require'json'
BaseIncludeDirs = {
  "C:/libs/lua/lua-5.3.5/src",
  "C:/Users/pholl/Documents/CodeProjects/HobyEngine/GameEngine/GameEngine",
  "C:/libs/imgui",
  "C:/libs/stb-master/stb-master",
  "C:/libs/glfw-3.2.1.bin.WIN64/include",
  "C:/libs/glm",
  "C:/libs/boostdir/boost_1_69_0",
  "C:/libs/boostdir/boost_1_69_0/stage/lib",
  "./Utilities"
}

function is_dir(path)
    file = io.open(path)
    return file == nil and string.find(path, '%.', 2)  == nil
end

function ScanDirs(directory)
    for dir in io.popen([[dir "]]..directory..[[" /b /ad]]):lines() do
      relativeDir = directory..'/'..dir
      if is_dir(relativeDir) then
        BaseIncludeDirs[#BaseIncludeDirs+1] = relativeDir
        ScanDirs(relativeDir)
      end
    end
end

function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

function LinkInProject(ProjName, Config, Plat)
    links("./" .. ProjName .. "/bin/" .. Plat .. "/"..Config .. "/" .. ProjName .. ".lib")
end

--print(#BaseIncludeDirs)
ScanDirs("./GameEngine")
ScanDirs("./Memory")
ScanDirs("./ReflectionSystem")
ScanDirs("./Utilities")
ScanDirs("./Debugger")
BaseIncludeDirs[#BaseIncludeDirs+1] = "./ReflectionSystem"
BaseIncludeDirs[#BaseIncludeDirs+1] = "./Graphics"
BaseIncludeDirs[#BaseIncludeDirs+1] = "./Memory"

AllIncludeDirs = BaseIncludeDirs
--print(#BaseIncludeDirs)
function tablelength(T)
  local count = 0
  for _ in pairs(T) do count = count + 1 end
  return count
end
--print("Include file list:")
--for i = 1, #BaseIncludeDirs do
--      print(BaseIncludeDirs[i])
--end
--print(os.winSdkVersion() .. ".0")
function TableConcat(t1,t2)
    for i=1,#t2 do
        t1[#t1+1] = t2[i]
    end
    return t1
end
function BaseProjSetup(ProjKind, Name, AdditionalIncludeDirs)
  systemversion(os.winSdkVersion()..".0")
  language "C++"
  kind(ProjKind)
  location("./" .. Name)
  files(baseIncludes)
  files {"./" .. Name .. "/**.h", "./" .. Name .. "/**.cpp" }
  includedirs(BaseIncludeDirs)
  includedirs(AdditionalIncludeDirs)
  if(AdditionalIncludeDirs ~= nil) then
	AllIncludeDirs = TableConcat(AllIncludeDirs, AdditionalIncludeDirs)
  end
  dependson "ReflectionParser"
  disablewarnings { "4244", "5030"}
  flags { "MultiProcessorCompile" }
end

function BaseLibSetup(Name, AdditionalIncludeDirs)
  BaseProjSetup("StaticLib", Name, AdditionalIncludeDirs)
end
workspace "GameEngine"
  configurations {"Debug", "Release"}
  platforms "Win64"
  libdirs
  {
    "C:/libs/boostdir/boost_1_69_0/stage/lib"
  }
  
  filter "configurations:Debug"
    cppdialect "C++17"
    defines { "DEBUG", "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE", "_CRT_SECURE_NO_WARNINGS", "_MBCS" }
    runtime "Debug"
    
  filter "configurations:Release"
    cppdialect "C++17"
    optimize "On"
    defines { "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE", "_CRT_SECURE_NO_WARNINGS", "_MBCS" }
    symbols "On"
    runtime "Release"
    
  filter { "platforms:Win64" }
    system "Windows"
    architecture "x64"
 
project "GameEngine"
  BaseProjSetup("WindowedApp", "GameEngine")
  entrypoint "mainCRTStartup"
  dependson { "Graphics", "Debugger", "Editor", "GUISystem", "Memory", "Physics", "ReflectionSystem", "Utilities" }
  filter "configurations:Debug"
    LinkInProject("Debugger", "Debug", "Win64")
    LinkInProject("Graphics", "Debug", "Win64")
    LinkInProject("Editor", "Debug", "Win64")
    LinkInProject("GUISystem", "Debug", "Win64")
    LinkInProject("Memory", "Debug", "Win64")
    LinkInProject("Physics", "Debug", "Win64")
    LinkInProject("ReflectionSystem", "Debug", "Win64")
    LinkInProject("Utilities", "Debug", "Win64")
    runtime "Debug"
  filter "configurations:Release"
    LinkInProject("Debugger", "Release", "Win64")
    LinkInProject("Graphics", "Release", "Win64")
    LinkInProject("Editor", "Release", "Win64")
    LinkInProject("GUISystem", "Release", "Win64")
    LinkInProject("Memory", "Release", "Win64")
    LinkInProject("Physics", "Release", "Win64")
    LinkInProject("ReflectionSystem", "Release", "Win64")
    LinkInProject("Utilities", "Release", "Win64")
    runtime "Release"
    
project "Graphics"
  BaseLibSetup("Graphics",  {
    "C:/libs/stb-master",
    "C:/libs/GLFW/glfw-3.2.1/include",
    "C:/libs/glad/include"
    })
  defines "STB_IMAGE_IMPLEMENTATION"
  links 
  {
    "C:/libs/glfw-3.2.1.bin.WIN64/lib-vc2015/glfw3.lib"
  }
  files "C:/libs/glad/src/glad.c"
  
project "Debugger"
  BaseLibSetup("Debugger")
  
project "Editor"
  BaseLibSetup("Editor", {
    "C:/libs/glad/include"
    })
  
project "GUISystem"
  BaseLibSetup("GUISystem")
  
project "Memory"
  BaseLibSetup("Memory")
  
project "Physics"
  BaseLibSetup("Physics")
  
project "ReflectionSystem"
  BaseLibSetup("ReflectionSystem")
  
project "Utilities"
  BaseLibSetup("Utilities")
    
project "ReflectionParser"
  BaseProjSetup("ConsoleApp", "ReflectionParser", {"C:/libs/llvm-project/clang/include", "C:/libs/rapidjson/include"})
  debugargs { "GameEngine.sln" }
  filter "configurations:Debug"
  links
  {
	"C:/libs/llvm-project/build/Debug/lib/libclang.lib"
  }
  filter "configurations:Release"
  links
  {
	"C:/libs/llvm-project/build/Release/lib/libclang.lib"
  }
  
project "ReflectionTest"
  BaseProjSetup("WindowedApp", "ReflectionTest")
  entrypoint "mainCRTStartup"
  dependson {"ReflectionSystem"}
  filter "configurations:Debug"
    LinkInProject("ReflectionSystem", "Debug", "Win64")
    runtime "Debug"
  filter "configurations:Release"
    LinkInProject("ReflectionSystem", "Release", "Win64")
    runtime "Release"

file = io.open ("includedirs.json", "w+")	
file:write(json.encode(AllIncludeDirs))
file:close()