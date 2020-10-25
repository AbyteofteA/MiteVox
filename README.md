```
 __  __ _ _     __     __        
|  \/  (_) |_ __\ \   / /____  __
| |\/| | | __/ _ \ \ / / _ \ \/ /
| |  | | | ||  __/\ V / (_) >  < 
|_|  |_|_|\__\___| \_/ \___/_/\_\

```
----------------------------------
MiteVox is an **Entity-Component-System**-based game engine. 
The engine is a result of experiments in Computer Graghics, Machine Learning and Computer Science in general. 

**Renderer** module currently supports only **OpenGL**, and the Renderer API is changing now.
Once the OpenGL Renderer implementation is ready the Renderer API will be freezed in order to preserve compatibility with other implementations. 

**FileIO** module contains the implementation of an object that reads and parses files **asynchronously**.

**AIModels** module was created in order to learn Machine Learning. At this point it has an implementation of 
**Feedforward Neural Networks** with **Backpropagation** learning algorithm for them. Also it has an implementation of 
**Convolutional layer**, but Backpropagation is not implemented for it yet. 

## Potential use cases
:video_game: Sandbox games

:rocket: Approximate physics simulations

:robot: AI's virtual playground

## Screenshot
![mitevox_demo](docs/mitevox_demo.png)

## :card_file_box: Features & TODO

:warning: - TODO

### ECSManager
- Entity Component System
- :warning: Prefabs
- :warning: Hierarchy
### FileIO
- Asynchronous file loader & parser
- Binary files
- Images (png, jpg)
- :warning: Audio files (ogg, mp3)
### Scripting
- Native (C/C++)
- :warning: Lua
### Math
- Data structures:
  - Buffer
  - Half-table
- Linear algebra:
  - Point3D
  - Vector3D
  - :warning: Matrices
- Numerical analysis:
  - Linear interpolation
- Graphs:
  - Trees:
    - Binary search tree
    - :warning: Quad tree
    - :warning: Octree
- Noise:
  - :warning: Perlin noise
  - :warning: OpenSimplex noise
### Renderer
- Primitives:
  - Point
  - Line
  - Triangle
- :warning: Sprites
- Skybox
- Model3D (+ .obj parser)
- Material:
  - Albedo map
  - Roughness map
  - Metallic map
  - :warning: Normal map
  - :warning: Glow map
  - :warning: .mtl parser
- Camera:
  - :warning: Focal length
- :warning: Multiple cameras
- Light:
  - Ambient light
  - Directional light
  - Point light
  - :warning: Spot light
- Multiple lights
- :warning: PBR
### AIModels
- MLModels:
  - FFNN (Feedforward Neural Networks)
  - FFNN + Backpropagation
  - CNN (Convolutional Neural Networks)
  - :warning: CNN + Backpropagation
  - :warning: CompositeNN
- Pathfinding:
  - :warning: A*
- :warning: StateMachine
### Physics
- Colliders:
  - Primitive collider
    - AABB intersection
    - :warning: SAT intersection
  - :warning: Mesh collider
- Rigid body
- :warning: Atoms
### Editors
- :warning: Color picker
- :warning: Function editor
- :warning: Material editor

## :bug: Known bugs & limitations
- [ ] If you assign light position to a camera position, z-component needs to be negated (why?).
- [ ] .obj parser doesn't support polygon grouping by material.
- [ ] .obj parser doesn't support polygon grouping by smoothing.
- [ ] Binary search tree has global variables.
- [ ] Transform::getDirectionVector() + SpotLight doesn't work as expected.

## :hammer_and_wrench: External dependencies (included)
- [GLEW - The OpenGL Extension Wrangler Library]( https://github.com/nigels-com/glew )
- [GLFW - A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input]( https://github.com/glfw/glfw )
- [GLM - is a header only C++ mathematics library for graphics software]( https://github.com/g-truc/glm )
- SOIL - Simple OpenGL Image Library

## :information_source: References
- [Learn OpenGL]( https://learnopengl.com )
- [Some materials]( http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html )
- [McGuire Computer Graphics Archive]( https://casual-effects.com/g3d/data10/index.html )
- [A Simple Entity Component System. Austin Morlan]( https://austinmorlan.com/posts/entity_component_system )
- [Neural networks. 3Blue1Brown]( https://www.youtube.com/watch?v=aircAruvnKk&list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi )

## :page_facing_up: License & copyright
This project is released under The MIT [License](LICENSE).

