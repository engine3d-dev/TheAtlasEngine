@REM glslc.exe builtin.shaders/test.vert -o builtin.shaders/test.vert.spv
@REM glslc.exe builtin.shaders/test.frag -o builtin.shaders/test.frag.spv
glslc.exe builtin.shaders/pbr.vert -o builtin.shaders/pbr.vert.spv
glslc.exe builtin.shaders/pbr.frag -o builtin.shaders/pbr.frag.spv

echo Finished Compiling GLSL Shaders