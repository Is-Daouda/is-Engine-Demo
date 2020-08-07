# is::Engine (Infinity Solutions::Engine) Demo

**2D platform game** in which you control a character who throws **Electro Shock** to destroy these enemies. To **complete a level** you must reach the **finish flag**.
It integrates `Intro Screen`, `Main Menu`, `Level`, `Game Over` and `Congratulation`. It comes with a **[Level Editor](https://github.com/Is-Daouda/is-Engine-Level-Editor)** that allows you to create your own levels and integrate them into the Demo.

![image](./images/demo_screen.png)

You can download the Android Demo **[![image](https://github.com/Is-Daouda/is-Engine-Demo/blob/master/CodeBlocks/data/image/gui/icon.png) Here](./demo-apk/is-Engine-v2.2-demo.apk)**

## About the project components
- These scenes : `Intro`, `Main Menu`, `Game Over` and `Congratulation` were created with the engine's standard tools
- A tool to configure the `Virtual Game Pad` (only for Android) has been integrated into the `Main Menu`
- An option to manage `sounds`, `music`, `vibration` (on Android), `game language` are in the `Main Menu`
- The `Level` scene was created with the `SDM (Step and Draw Manager)` a tool which **automatically manages the window and the objects of the scene by updating and displaying them**
- The configuration of the `keyboard keys` (Windows / Linux) and `the project` (window size, title, author, file path, ...) can be found in `GameConfig.h`

## Example of a project created with the engine
- [I Can Transform](https://play.google.com/store/apps/details?id=com.isdaouda.icantransform&hl=En)

## In this directory you have four (4) projects :
- **AndroidStudio**        : for Android development with Android Studio
- **CodeBlocks**           : for PC development with Code::Blocks
- **CMake**           	   : for PC development with CMake
- **VSCode**               : for PC development with Visual Studio Code

## Prerequisites
- SFML Library (2.4 +)
- GCC Compiler (7.3 +)

**AndroidStudio**
- Android Studio (3.1.3 +)
- Android SDK and NDK (r20b)
- Firebase C++ SDK 4.5.0 (If you need it)

**CodeBlocks**
- Code::Blocks (20.03)

**CMake**
- CMake (3.1 +)

**VSCode**
- Visual Studio Code (1.42.0 +)
- Git Bash (For Windows only)

## Todo
Everything has already been configured all you need to do in relation to the tree (3) projects is to link the libraries

## Description of the project structure:
![header](./images/is_Engine_structure.png)
----------------------------
#### `main.cpp` file
Contains the entry point of the program, inside there are two instructions :
- `game.play()`: Launches the engine rendering loop which allows to manage the introduction screen, main menu, level and game over.
- `game.basicSFMLmain()` (disabled by default): Launches the display of a classic SFML window. The implementation is in the `basicSFMLmain.cpp` file. *Very useful if you already have a project under development and you want to associate it with the engine. You can also use it to implement your own components to the engine.*

----------------------------
#### `app_src` folder
Contains the source code of the game.
Description of these sub-directories:
- `activity` : Contains the `Activity` class which allows the interaction of the different scenes of the game
- `config`   : Contains the `GameConfig.cpp` file which allows to define the general parameters of the game
- `gamesystem_ext` : Contains a class derived from `GameSystem` which allows to manipulate game data (save, load, ...)
- `language` : Contains the `GameLanguage.cpp` file which allows to manage everything related to game languages
- `levels`   : Contains game levels and the `Level.h` file which allows to integrate them into the game
- `objects`  : Contains the objects that will be used in the different scenes
- `scenes`   : Contains the different scenes of the game (Introduction, Main menu, ...)

----------------------------
#### `data` folder
Contains game resource files (music, sound sfx, image, ...)

----------------------------
#### `isEngine` folder
Contains the source code of the game engine

## Contribute
You have any ideas ? Well don’t hesitate to participate in the development of this engine. Because one of the objectives of this project is to form a community that will work on the engine to make it more efficient and easy to use for everyone.

## Contacts
  * For any help or other contribution please contact me on my [email address](mailto:isdaouda.n@gmail.com)
  * You can follow me on Twitter for more informations on my activities [@Is Daouda Games](https://twitter.com/IsDaouda_Games)