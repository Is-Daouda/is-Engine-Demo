# is::Engine (Infinity Solutions::Engine) 3.3.3 Demo
- This Demo is about an iconic game that marked a whole generation and which celebrates its **35th anniversary** this year: It's **Super Mario Bros NES**!
- It comes with a **[Level Editor](https://github.com/Is-Daouda/is-Engine-Level-Editor)** that allows you to create your own levels and integrate them into the game.

![image](./images/demo_screen.png)

## About the project components
- These scenes : `Intro`, `Main Menu` and `Game Over` were created with the engine's standard tools
- A tool to configure the `Virtual Game Pad` (only for Android) has been integrated into the `Main Menu`
- An option to manage `sounds`, `music`, `vibration` (on Android), `game language` are in the `Main Menu`
- All `scenes` were created with the `SDM (Step and Draw Manager)` a tool which **automatically manages the window and the objects of the scene by updating and displaying them**
- The configuration of the `keyboard keys` (Windows / Linux) and `the project` (window size, title, author, file path, ...) can be found in **[GameConfig.h](./app/src/main/cpp/app_src/config/GameConfig.h)**

## Example of a project created with the engine
- [I Can Transform](https://play.google.com/store/apps/details?id=com.isdaouda.icantransform&hl=En)
- [Pac-Pac Prandial War](https://www.youtube.com/watch?v=7CXkiD_jTRg&t=25s&authuser=0)
- [Soldier VS Zombies](https://www.youtube.com/watch?v=qK-Aj-7-czM&authuser=0)

## Prerequisites
- [SFML Library (2.4 +)](https://www.sfml-dev.org/download.php)
- [SDL 2 (2.0.12 +)]()
- GCC Compiler (7.3 +)

---

## How to use is::Engine with the different development tools:
## ![android](https://i48.servimg.com/u/f48/20/16/75/27/icon_a10.png) Android Studio
This project uses the template of **[Georgik](https://github.com/georgik/sdl2-android-example)** and **[Lauchmelder23](https://github.com/Lauchmelder23/SDLU)**.

**1. Prerequisites**
- Android Studio (4.0.1 +)
- Android SDK and NDK (r20b)

## ![danger](https://i48.servimg.com/u/f48/20/16/75/27/icon_d10.png) Very important
- On Android SFML games run with SDL library. If you want to use SDL functions in your source code, use the **IS_ENGINE_SDL_2 macro**.
- The audio format supported at the moment is **.WAV**
- Some SFML functions like: **Vertex Array, Render Texture** are not yet supported. These additions will be made soon!
- **Your help to improve the engine will be welcome!**
- [Please read this](#Contribute).

**2. Installation**
##### Windows
1. Download [Android Studio 3.x](https://developer.android.com/studio) (recommended version 4.0.1).
2. Download the [Android SDK](https://developer.android.com/studio) and install it in **C:/Android/SDK**.
3. Download [Android NDK android-ndk-r20b-windows-x86_64](https://developer.android.com/ndk/downloads/older_releases.html) and create a folder on your disk as follows **C:/Android/NDK** then extract the contents of the zip in this folder.
4. Set the environment variable **ANDROID_NDK** with the path **C:/Android/NDK**.
6. Move the **is-Engine-Demo** project to your **C:/ (C:/is-Engine-Demo)**.
7. Open the **is-Engine-Demo** folder with **Android Studio** and start the compilation.

If all goes well you will have a **is::Engine Demo Screen** on your **Android emulator**.

![Image](./images/demo_screen.png)

**Enjoy!**

**2. How to replace the package name (com.author.isengine) of the application**
- Follow these steps carefully. A single error and the application will crash wonderfully when launching on emulator / mobile!
1. Replace this line in the [gradle.app](./app/build.gradle#L32) file.
2. Replace this line in the [AndroidManifest.xml](./app/src/main/AndroidManifest.xml#L3) file.
3. Replace this line in the [SDLActivity.java](./app/src/main/java/com/author/isengine/SDLActivity.java#L1) file.
4. Replace the abresence **[com/author/isengine](./app/src/main/java/com/author/isengine/)** in which is the file [SDLActivity.java](./app/src/main/java/com/author/isengine/SDLActivity.java#L1) that you have just modified at the top by yours (example **com/yourname/yourgamename**).
5. Replace this part **..._ com_author_isengine _...** of line [20](./app/src/main/cpp/SDL_android_main.c#L20) and [23](./app/src/main/cpp/SDL_android_main.c#L23) in the file [SDL_android_main.c](./app/src/main/cpp/SDL_android_main.c#L20) by yours (example **com_yourname_yourgamename)**.
6. Replace this part **..._ com_author_isengine _...** on the 23 lines of the file [SDL_android.c](./libs/SDL2/src/core/android/SDL_android.c#L156) by yours (example **com_yourname_yourgamename**).
- **I strongly advise you to use the replace function of your text editor** (on Notepad++ we use Ctrl + F + Replace option).
7. Replace this line in the [GameConfig.h](./app/src/main/cpp/app_src/config/GameConfig.h#L148) file.
- Note that this part is only required if you want to use the game engine data save / load functions.

**3. Adding Source Files**
- So that Android Studio can detect your source files (.cpp) you must include them in the **[app_src.cmake](./app/src/main/cmake/app_src.cmake) or [isengine.cmake](./app/src/main/cmake/isengine.cmake)** file which is located in the **[is-Engine-Demo/app/src/main/cmake](./app/src/main/cmake/)** location.

**4. Application location**
- The application can be found in **is-Engine-Demo/app/build/outputs/apk**.

---

## ![web](https://i48.servimg.com/u/f48/20/16/75/27/icon_w10.png) Web (HTML 5 - CSS 3)
- If you want to make your SFML project compatible with the Web (Be able to run it in a web browser), please watch this **[video tutorial](https://youtu.be/x_YQLHoPMbc)**.

**1. Prerequisites**
- Emscripen (1.39.7 +)
- Python (3.8.1 +)
- CMake (3.1 +)
- Java
- SDL 2 **(It is downloaded with the internet connection when executing commands)**

**2. Installation**
##### Windows
1. Download [Emscripten](https://github.com/emscripten-core/emsdk) and install it in **C:/emsdk**, define its path in the environment variable **Path**
2. Download [Python](https://www.python.org/downloads/release/python-381/) after installation, define its path in the environment variable **Path**
3. Download [CMake](https://cmake.org/download/) after installation, define its path in the environment variable **Path**
4. Download [Java](https://www.oracle.com/java/technologies/javase-jre8-downloads.html) after installation, define its path in the environment variable **Path**
5. Move the **is-Engine-Demo** project to your **C:/ (C:/is-Engine-Demo)**.
6. Execute this command :
```bash
cd c:/is-Engine-Demo/app/src/main
mkdir bin-web
cd bin-web
emsdk activate latest
emcmake cmake ..
make
python -m http.server
```
7. Visit this url **localhost:8000** in your **Web Browser**.

If all goes well you will have a **is::Engine Demo Screen** on your **Web Browser**.

![Image](./images/demo_screen.png)

**Enjoy!**

**3. Adding Source Files**
- In order for CMake to detect your source files (.cpp) you must include them in the **[app_src.cmake](./app/src/main/cmake/app_src.cmake) or [isengine.cmake](./app/src/main/cmake/isengine.cmake)** file which is located in the **[is-Engine-Demo/app/src/main/cmake](./app/src/main/cmake/)** location.

## ![danger](https://i48.servimg.com/u/f48/20/16/75/27/icon_d10.png) Very important
- **is::Engine** works on the Web thanks to **SDL 2**.
- These libraries: **TMXLite, TMXLoader, TinyFileDialog** are not supported in the web version of is::Engine.
- If you want to use SDL functions in your source code, use the **IS_ENGINE_SDL_2 macro**.
- Note that some SFML functions like: **Vertex Array, Render Texture** are not yet supported. These additions will be made soon!

---

## ![Web Push Notification](https://i48.servimg.com/u/f48/20/16/75/27/notif_10.png) Web Push Notification
- If you want to make your SFML project compatible with the Web (Be able to run it in a web browser), please watch this **[video tutorial](https://youtu.be/x_YQLHoPMbc)**.

- ![danger](https://i48.servimg.com/u/f48/20/16/75/27/icon_d10.png)<br>
By default the web push notification has been disabled. Because to make it work you must have an internet connection. In case there is no internet access and it is not well launched, it can prevent the execution of the web program.<br>
To enable it, please modify these lines in **[index.html](./app/src/main/web/index.html)** : **[9](./app/src/main/web/index.html#L9), [25](./app/src/main/web/index.html#L25), [68](./app/src/main/web/index.html#L68)**

#### Installation
- This shows how to test the push notification. Note that normally to use it, you have to associate it with a database (backend). But here we will use it with the **Push Companion** site **(It will serve as a backend for us!)**.
- For more information on Push Notification please see this [page](https://developers.google.com/web/fundamentals/codelabs/push-notifications).
1. Web browser ([preferably Google Chrome](https://www.google.fr/chrome/?brand=CHBD&brand=XXVF&gclid=EAIaIQobChMI7a315b6c7gIVEKSyCh0O8QJjEAAYASABEgJfd_D_BwE&gclsrc=aw.ds))
2. [Web server for Chrome](https://chrome.google.com/webstore/detail/web-server-for-chrome/ofhbbkphhbklhfoeikjpcbhemlocgigb) or your own web server.
3. Define the **Application Server Keys** in the [main.js](./app/src/main/web/scripts/main.js#L25) file. You can get this keys (We will use the public key) [here](https://web-push-codelab.glitch.me/).
4. Launch the **[web](./app/src/main/web)** or **bin-web folder (generate using emscripten)** via the web server.
5. Click on the **"Enable Push Messaging" button** to activate the sending of Push Notifications. Once done you will have a code (which can be used to send you push notifications).
6. Go to this [site](https://web-push-codelab.glitch.me/) and enter the code in the **"Subscription to Send To" text field** followed by your message in **"Text to Send" text field**. Click on the **"Send Push Message" button**.
7. If all goes well you will have a push notification followed by the message you sent in the console (development tool) of your browser.

![image](https://i.servimg.com/u/f48/20/16/75/27/image10.jpg)

#### Configure the Push Notification
1. To change the **information (title, details, ...)** of the Push Notification you must refer to the [sw.js](./app/src/main/web/sw.js#L28) file.
2. To change the **Push Notification image** files, refer to the [images](./app/src/main/web/images) folder.
3. To change the **page that is launched** when you click on the notification, refer to the [sw.js](./app/src/main/web/sw.js#L45) file.

---

## ![cmake](https://i48.servimg.com/u/f48/20/16/75/27/icon_c11.png) CMake
**1. Prerequisites**
- CMake (3.1 +)

**2. Installation**
#### Windows
1. Compile SFML with CMake to have **static libraries** and put on **C:/ (C:/SFML)**.
2. Move the **is-Engine-Demo** project to your **C:/ (C:/is-Engine-Demo)**.
3. Execute this command :
```bash
cmake -S "C:/is-Engine-Demo" -B "C:/build"
cd build
make
```

#### Linux
1. Install SFML 2.5.1 on your machine.
2. Move the **is-Engine-Demo** project to **/home/user/ (/home/user/is-Engine-Demo)**.
3. Execute this command :
```bash
sudo cmake -S "is-Engine-Demo" -B "build"
cd build
sudo make
```

**3. After installation**
- You will have a **bin** folder in which the engine demo is located.

**5. Adding Source Files**
- In order for CMake to detect your source files (.cpp) you must include them in the **[app_src.cmake](./app/src/main/cmake/app_src.cmake) or [isengine.cmake](./app/src/main/cmake/isengine.cmake)** file which is located in the **[is-Engine-Demo/app/src/main/cmake](./app/src/main/cmake/)** location.

---

## ![qt](https://i48.servimg.com/u/f48/20/16/75/27/qt_ico10.png) Qt

**1. Installation**
#### Windows
1. Download Qt 5.12.9 MinGW [here](http://qtproject.mirror.liquidtelecom.com/archive/qt/5.12/5.12.9/qt-opensource-windows-x86-5.12.9.exe) and install it.
2. Download this [version of SFML](https://github.com/Is-Daouda/SFML_Qt_MinGW) already compiled for Qt 5.12.9 and extract it in **C:/ (C:/SFML_Qt_MinGW)**.

**2. Opening the project with the IDE:**

#### Windows
1. Run the file **open_qt_creator.bat** in the main directory *(Make sure you have included the path to the Qt executable in your PATH environment variable)*.
2. Or open the file **is-Engine.pro** in the location **[is-Engine-Demo/app/src/main/qt](./app/src/main/qt/)**

**3. Executable location**
- The compiler files can be found in **is-Engine-Demo/app/src/main/bin-Qt**.

---

## ![vs](https://i48.servimg.com/u/f48/20/16/75/27/icon_v10.png) Visual Studio Code
This project uses the template of **andrew-r-king**. For more information on this template [click here](https://github.com/andrew-r-king/sfml-vscode-boilerplate).

**1. Prerequisites**
#### Windows
- [SFML 2.5.1 - GCC 7.3.0 MinGW (DW2) 32-bit](https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-32-bit.zip)
- [GCC 7.3.0 MinGW (DW2) 32-bit](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download)
- [Git Bash](https://git-scm.com/downloads)

#### Linux
- Get SFML 2.5.1 from your distro if it has it, or compile from source.

**2. Installation**
#### Windows
1. Download & Extract SFML 2.5.1 to **C:/SFML/** where the bin/lib/include folders are contained within.
2. Download & Extract MinGW to **C:/mingw32/** where the bin/lib/include folders are contained within.

#### Linux
1. Ensure the GCC Toolchain is installed (**sudo apt install build-essential**).
2. Run **sudo apt install libsfml-dev**. The SFML version you got will vary depending on the distro. 2.5.1 is included in [Ubuntu 19.04 Disco Dingo](http://cdimage.ubuntu.com/daily-live/current/HEADER.html) for example.

**3. Opening the project with the IDE:**
#### Windows
1. Run the file **open_vscode.bat** in the main directory.

#### Linux
2. Execute this command in the main directory:
```bash
code -n "./app/src/main"
```

#### All
3. Or open the **[is-Engine-Demo/app/src/main](./app/src/main/)** folder with **Solution Explorer**.

**4. Executable location**
- The compiler files can be found in **is-Engine-Demo/app/src/main/bin-vscode**.

---

## ![cb](https://i48.servimg.com/u/f48/20/16/75/27/icon_c10.png) Code::Blocks

**1. Installation**
#### Windows
1. Download Code::Blocks 20.03 MinGW [here](https://sourceforge.net/projects/codeblocks/files/Binaries/20.03/Windows/codeblocks-20.03mingw-setup.exe/download) and install it.
2. Download this [version of SFML](https://github.com/Is-Daouda/SFML_CB_MinGW) already compiled for Code::Blocks 20.03 and extract it in **C:/ (C:/SFML_CB_MinGW)**.

#### Linux
1. Download Code::Blocks 20.03 and install it.
2. Ensure the GCC Toolchain is installed (**sudo apt install build-essential**).
3. Run **sudo apt install libsfml-dev**. The SFML version you got will vary depending on the distro. 2.5.1 is included in [Ubuntu 19.04 Disco Dingo](http://cdimage.ubuntu.com/daily-live/current/HEADER.html) for example.

**2. Opening the project with the IDE:**

#### Windows
1. Run the file **open_codeblocks.bat** in the main directory *(Make sure you have included the path to the Code::Blocks executable in your PATH environment variable)*.
2. Or open the file **is-Engine-windows.cbp** in the location **[is-Engine-Demo/app/src/main](./app/src/main/)**

#### Linux
1. Execute this command in the main directory:
```bash
codeblocks "./app/src/main/is-Engine-linux.cbp"
```
2. Or open the file **is-Engine-linux.cbp** in the location **[is-Engine-Demo/app/src/main](./app/src/main/)**.

**3. Executable location**
- The compiler files can be found in **is-Engine-Demo/app/src/main/bin-codeblocks**.

---

## ![cb](https://i48.servimg.com/u/f48/20/16/75/27/icon_c10.png) Develop SFML games with SDL 2

**1. Installation**
#### Windows
1. Download Code::Blocks 20.03 MinGW [here](https://sourceforge.net/projects/codeblocks/files/Binaries/20.03/Windows/codeblocks-20.03mingw-setup.exe/download) and install it.
2. Download this [version of SDL 2](https://github.com/Is-Daouda/SDL2) and extract it in **C:/ (C:/SDL2)**.
3. Put the **.dll files** which is in the **bin** folder of SDL2 in the **[main](./app/src/main/)** folder.

**2. Opening the project with the IDE:**
1. Run the file **open_codeblocks_sdl.bat** in the main directory *(Make sure you have included the path to the Code::Blocks executable in your PATH environment variable)*.
2. Or open the file **is-Engine-windows-SDL2.cbp** in the location **[is-Engine-Demo/app/src/main](./app/src/main/)**

**3. Executable location**
- The compiler files can be found in **is-Engine-Demo/app/src/main/bin-codeblocks**.

## ![danger](https://i48.servimg.com/u/f48/20/16/75/27/icon_d10.png) Very important
- If you want to use SDL functions in your source code, use the **IS_ENGINE_SDL_2 macro**.
- Note that some SFML functions like: **Vertex Array, Render Texture** are not yet supported. These additions will be made soon!

---

## ![icon](https://i48.servimg.com/u/f48/20/16/75/27/icon10.png) Change application icon:
#### Android
- To change the icon of the application you must go to the location **[is-Engine-Demo/app/src/main/res](./app/src/main/res/)** replace all the images (PNG) which are in the **drawable** subfolders.

#### Web (HTML 5 - CSS 3)
- To change the icon of the application you must go to the location **[is-Engine-Demo/app/src/main/web](./app/src/main/web/)**.

#### Windows
- To change the icon of the application you must go to the location **[is-Engine-Demo/app/src/main/env/windows](./app/src/main/env/windows)** replace all the images **(Attention CMake uses the same resources).**

#### Linux
- To change the icon of the application you must go to the location **[is-Engine-Demo/app/src/main/env/linux](./app/src/main/env/linux)**.

---

## Description of the project structure:
![header](./images/is_Engine_structure.png)
----------------------------
- The source files of the project can be found in the **[is-Engine-Demo/app/src/main/cpp](./app/src/main/cpp/)** location.

#### 1. [main.cpp](./app/src/main/cpp/main.cpp) file
Contains the entry point of the program, inside there are two instructions :
- `game.play()`: Launches the engine rendering loop which allows to manage the introduction screen, main menu, level and game over.
- `game.basicSFMLmain()` (disabled by default): Launches the display of a classic SFML window. The implementation is in the **[basicSFMLmain.cpp](./app/src/main/cpp/basicSFMLmain.cpp)** file. *Very useful if you already have a project under development and you want to associate it with the engine. You can also use it to implement your own components to the engine.*

----------------------------
#### 2. [app_src](./app/src/main/cpp/app_src/) folder
Contains the source code of the game.
Description of these sub-directories:
- **[activity](./app/src/main/cpp/app_src/activity/)** : Contains the **[Activity](./app/src/main/cpp/app_src/activity/GameActivity.h)** class which allows the interaction of the different scenes of the game.
- **[config](./app/src/main/cpp/app_src/config/)** : Contains the **[GameConfig.h](./app/src/main/cpp/app_src/config/GameConfig.h)** file which allows to define the general parameters of the game. It also contains the file **[ExtraConfig.h](./app/src/main/cpp/app_src/config/ExtraConfig.h)** which allows to activate / deactivate certain engine functionality (Engine optimization, SDM, Admob, Main Render Loop, ...).
- **[gamesystem_ext](./app/src/main/cpp/app_src/gamesystem_ext/)** : Contains **[GameSystemExtended](./app/src/main/cpp/app_src/gamesystem_ext/GameSystemExtended.h)** a class derived from **[GameSystem](./app/src/main/cpp/isEngine/system/function/GameSystem.h)** which allows to manipulate game data (save, load, ...).
- **[language](./app/src/main/cpp/app_src/language/)** : Contains the **[GameLanguage.h](./app/src/main/cpp/app_src/language/GameLanguage.h)** file which allows to manage everything related to game languages.
- **[levels](./app/src/main/cpp/app_src/levels/)** : Contains game levels and the **[Level.h](./app/src/main/cpp/app_src/levels/Level.h)** file which allows to integrate them into the game.
- **[objects](./app/src/main/cpp/app_src/objects/)** : Contains the objects that will be used in the different scenes.
- **[scenes](./app/src/main/cpp/app_src/scenes/)** : Contains the different scenes of the game (Introduction, Main menu, ...).

----------------------------
#### 3. [assets](./app/src/main/assets/) folder
Contains game resource files (music, sound sfx, image, ...)

----------------------------
#### 4. [isEngine](./app/src/main/cpp/isEngine/) folder
Contains the source code of the game engine

---

## Special things to know about the engine
### is::LibConnect
With the is::LibConnect you can write code for a specific library. Here is how to do it:
```cpp
sf::Text text;
text.setString(
// on PC (Windows / Linux)
#if define(IS_ENGINE_SFML)    
    "We use SFML 2 library"

// on Android or when you use SDL to create SFML games on PC (only for Code::Block at the moment)    
#elif define(IS_ENGINE_SDL_2)
    "Run SFML 2 with SDL 2"

// When we develop for the web (HTML 5) with Emscripten    
#elif define(IS_ENGINE_HTML_5)
    "SFML 2 on Web"
#endif
              );
```

#### If you have discovered another way to use the game engine, don't hesitate to share it! We will put it in this Special section so that other people can benefit from it!

---

## How to update an is::Engine project
1. First of all the part of is::Engine that changes most often during updates is the [isEngine](./app/src/main/cpp/isEngine/) folder. But it also happens that these files can be modified:
- [GameActivity.h](./app/src/main/cpp/app_src/activity/GameActivity.h)
- [GameConfig.h](./app/src/main/cpp/app_src/language/GameLanguage.h)
- [ExtraConfig.h](./app/src/main/cpp/app_src/config/ExtraConfig.h)
- [GameSystemExtended.h](./app/src/main/cpp/app_src/gamesystem_ext/GameSystemExtended.h)
- [basicSFMLmain.cpp](./app/src/main/cpp/basicSFMLmain.cpp)
- [GameLanguage.h](./app/src/main/cpp/app_src/language/GameLanguage.h)
- And the files which is in [cmake](./app/src/main/cmake) and [web](./app/src/main/web) folder.
- ![danger](https://i48.servimg.com/u/f48/20/16/75/27/icon_d10.png) **So watch them carefully in case you encounter any errors during migration!**
2. To update your old project with a new version of is::Engine: the files (.h and .cpp) you need to move are in [objects](./app/src/main/cpp/app_src/objects/) and [scenes](./app/src/main/cpp/app_src/scenes/). **Note that these folders never change whatever the version!**

---

## Contribute
- If you want to participate in the development of the project to help me improve the engine, please note that you are welcome! Together we go further!
- One of the objectives of this project is to create a large community that can work on the engine to allow many people around the world to easily realize their dream games / applications!

## Contacts
  * For any help please contact me on my [email address](mailto:isdaouda.n@gmail.com)
  * You can follow me on Twitter for more informations on my activities [@Is Daouda Games](https://twitter.com/IsDaouda_Games)