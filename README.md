# template.axl.game.1

A basic template for starting up with a simple 2D project.

## Dependencies

- [axl.game](https://github.com/defalt8/axl.game.git)
  - [axl.glw](https://github.com/defalt8/axl.glw.git)
  - [axl.math](https://github.com/defalt8/axl.math.git)
  - [axl.util](https://github.com/defalt8/axl.util.git)

## Requirements

- Windows operating system. (Linux and other platforms implementations are on the way)
- CMake 3.10 or above.
- Any C and C++ SDK that can be accessed through a command line. (MSVC, MinGW) If you have Visual Studio or CodeBlocks then you are set.

## How to get started

- Clone this repos into the same directory.
  - git clone https://github.com/defalt8/template.axl.game.1.git
  - git clone https://github.com/defalt8/axl.game.git
  - git clone https://github.com/defalt8/axl.glw.git
  - git clone https://github.com/defalt8/axl.math.git
  - git clone https://github.com/defalt8/axl.util.git
  - cd template.axl.game.1
  - mkdir out\\build
  - cd out\\build
  - cmake -B . -S ../../ (or with a generator of your choosing.) cmake -G \<generator\> -B . -S ../../
  - cmake --build .
  
