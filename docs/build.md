# Build Guide

## Design Philosophy

The aim for this project was to build a super lightweight, self-contained and portable executable. Thus, all libraries have been linked statically and all required resources embedded directly in the executable.

Why? Partly for the challenge, and partly to show that applications can still be built without all the bloat that has become so commonplace nowadays.

## Dependencies

### ENet

**Required for networking.**

This is distributed as a header-only library, so including it is trivial.

More information [here](https://github.com/zpl-c/enet#installation).

### GLEW

**Required for OpenGL support.**

GLEW ships with a Visual Studio project for building it as a static library (`glew_static.vcxproj`).

Crucially, projects linking with this library must also add the `GLEW_STATIC` preprocessor definition, as described [here](http://glew.sourceforge.net/install.html).

### GLFW

**Required for window creation and input handling.**

This can be built using CMake as described [here](https://www.glfw.org/docs/3.3/compile.html#compile_generate).

In short, CMake requires 2 directories to be specified:

- For the source code directory, use the root folder of the library
- For the output binaries, create a `build` subfolder in the root folder of the library

After pressing "Configure", some flags can be tweaked before generating the project. These are described [here](https://www.glfw.org/docs/3.3/compile.html#compile_options). Crucuially, `USE_MSVC_RUNTIME_LIBRARY_DLL` should be unticked to use the static library version of the Visual C++ runtime library.

Finally, click "Generate" to create the Visual Studio project that can build the static library (`glfw.vcxproj`).

### GLM

**Required for matrix and vector operations.**

This is distributed as a header-only library, so including it is trivial.

More information [here](https://github.com/g-truc/glm/blob/master/manual.md#-11-using-global-headers).
