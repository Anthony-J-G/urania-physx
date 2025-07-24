Design
==========

This repository is a collection and exploration of various fluid simulation methodologies with a strong focus on real-time application. Additionally the repository carries with it an interactive editor for manipulating and compiling the client simulations the editor is running. This opens up the opportunity for implementing and trying out different dynamic library and hot reloading techniques.

The project is subdivided into 5 different sections, the Runtime, the Editor, the Engine, the Simulations, and the Documentation.

Runtime
-------
*Executable*

The runtime is the main executable that controls different aspects of the project. As it's name suggests, it is the default manager of the application's runtime. This includes opening the editor as necessary, hot reloading the simulations or engine, and managing the backend/platform. Currently the main backend utilizes a dependency on [raylib](https://www.raylib.com/).

Editor
------
*Library*

The editor is an interactive window into both the engine and the implementation simulations. By default the editor is compiled into the runtime and is automatically opened when the application is run. The engine has a dependency on [dear imgui](https://github.com/ocornut/imgui) as well as [rlImGui](https://github.com/raylib-extras/rlImGui) to allow for rendering the UI while using `raylib` as the backend.

Engine
------
*Library*

The engine contains generic code that is necessary across implementations. This includes a basic physics engine as well as other potentially useful features such as a linear algebra library. By default the engine is connected to the runtime and simulations dynamically. This allows for the engine code to be modified while the editor/runtime is running and reloaded during runtime. However, this can be modified so that everything is statically linked at compile time, or alternatively during load time. 

Simulations
-----------
*Library*

The simulations contain the actual implmenetations of each specific fluid simulation along with whatever metrics that are necessary to collect. By default the simulations are linked during runtime to allow for recompilation and "hot reloading" during runtime, but this can be adjusted as a compile time option.