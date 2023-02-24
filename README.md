
# MiteVox

MiteVox is a 3D game engine (or rather a game framework so far). 
The engine is a result of experiments in Computer Graghics, Machine Learning and Computer Science in general.

## How to build
The project is built with ``Visual Studio 2022``.

## Potential use cases
:video_game: Sandbox games

:rocket: Approximate physics simulations

:robot: AI's virtual playground

## Screenshots
![](docs/mitevox_demo.png)

## Structure & features

### Demo project(s)
The solution contains an example project: `sandbox / DemoProject`

### engine / MiteVox
The core of the engine. It defines the **entry point**.
- Skeletal animations
- Morph target animations (per-vertex animations)
- engine_config.json

### engine / Renderer
- Rendering APIs: OpenGL
- Multiple lights

### engine / FileIO
Contains implementation of asynchronous file loader & parser.

### engine / Math
- Vectors, Matrices, Quaternions
- Data structures: Buffer, Half-table
- Numerical analysis:
  - Interpolation: Linear
- Graphs:
  - Trees: Binary search tree, Octree

### engine / AIModels
- ML-models:
  - Feedforward Neural Networks
    - FullyConnectedLayer (+ backpropagation)
    - SubsamplingLayer2D
    - SoftmaxLayer
  - Recurrent Neural Networks
    - HopfieldNetwork
  - NeuralNetwork: class that joins different types of NNs

### engine / Physics
Has an implementation of AABB collisions and defines rigid body.

### engine / UIEventHandler
Has a basic user I/O setup. It definitely needs redesign...

## :bug: Known bugs & limitations :bug:
- [ ] Skinned mesh uses global transform of the node that instantiates the mesh instead of global transform of the skeleton root.
- [ ] InterpolationTest : collider calculation anomaly
- [ ] .obj parser doesn't support polygon grouping by material.
- [ ] .obj parser doesn't support polygon grouping by smoothing.
- [ ] Binary search tree has global variables.
- [ ] The project should be built with CMake.

## External dependencies (included)
- [GLEW (The OpenGL Extension Wrangler Library)](https://github.com/nigels-com/glew)
- [GLFW (A multiplatform library for OpenGL)](https://github.com/glfw/glfw)
- [GLM (A header only C++ mathematics library)](https://github.com/g-truc/glm)
- SOIL (Simple OpenGL Image Library)

## References
1. [Learn OpenGL](https://learnopengl.com)
2. [Some materials](http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html)
3. [McGuire Computer Graphics Archive](https://casual-effects.com/g3d/data10/index.html)
4. [A Simple Entity Component System. Austin Morlan](https://austinmorlan.com/posts/entity_component_system)
5. [JSON Standard (ECMA 404)](https://www.ecma-international.org/publications-and-standards/standards/ecma-404/)
6. [Neural networks. 3Blue1Brown](https://www.youtube.com/watch?v=aircAruvnKk&list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi)

