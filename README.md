# Animal Tracker for Red Dead Redemption 2

This is the github project for the Animal Tracker mod. For instruction & help about mod usage head over to the [Nexus Mods page](https://www.nexusmods.com/reddeadredemption2/mods/6827)

## Building

### Prerequisites

The build process uses [Conan](conan.io) for dependency management. Conan is best installed through [pip](https://pypi.org/project/pip/). The project comes with a conan profile for an msvc compiler, so I recommend a Visual Studio installation on your system. However, we will be using cmake to generate our build system, so other compilers should work as well. If you know what you are doing you can also use Ninja, VSCode, Clang, .... As RDR2 is a Windows Game this project will require a Windows PC for building.

### Setup Dependencies & Generate Build System

With conan installed you will first need to generate a default profile if you have not already. Do so with `conan profile detect`. 

Now let conan build dependencies for you. Navigate to the project root and run: 
```pwsh
conan install . --build missing --lockfile resources/conan/conan.lock -s compiler.cppstd=20 -s compiler.cstd=11 -s build_type=Release
```
This will take a short while, but if everything went well, thare should now be a new "build" directory. 

Make sure you are in a cmd terminal (not powershell) and run the generated `build/generators/conanbuild.bat` snippet. If you see no output everything worked. This needs to run everytime you open a new terminal that you want to work with.

Now, still from the project root dir, run the cmake command that the conan install call printed at the end of the previous step and add two additional arguments. For me the call was `cmake --preset conan-default` and now we add two more arguments to that call: First, there is no point in a Debug mode as we cannot hook a debugger to RDR2, so we turn that of with `-DCMAKE_CONFIGURATION_TYPES=Release`. If you are using a non-Visual Studio generator, instead write `-DCMAKE_BUILD_TYPE=Release`. Second, if we don't want to manually move all files later on to the game directory, we set it's path as install prefix, so that we can use the cmake install command to copy our built project & configuration files to the right location. Your game location will be somewhere like C:\Program Files (x86)\Steam\steamapps\common\Red Dead Redemption 2.

So for me, the complete cmake call is this:
```
cmake --preset conan-default -DCMAKE_CONFIGURATION_TYPES="Release" -DCMAKE_INSTALL_PREFIX="C:\Program Files (x86)\Steam\steamapps\common\Red Dead Redemption 2"
```

Now you are all setup to work on or build the project. If you have Visual Studio installed, you should have a .sln file in the build folder that you can work with. Just be aware that this solution file is temporary, specific to your machine, will not be under version control and any changes made to the project settings will be overwritten if you don't add them to the CMakeLists.txt files.

### Build & Run

In Visual Studio you can use the GUI to build and the INSTALL target to install.

From the command line, use `cmake --build build` to build from the project root, and `cmake --install build` to install the build artifacts to your Red Dead folder. 

After that is run, simply start the game through Steam or wherever.

# Credits 

This project started out as a fork of Toxin4ick's "RDR2 Animals Finder" and has since then expanded on the scope. Be sure to check out his original version of the mod.

Alexander Blade for developing the RDR2 Script hook used to inject this mod into the game aswell as the rest of the modding community for their research into the inner workings of the game, providing resources and making guides.

# Contributions

If you want to contribute feel free to create an issue or open a pull request with your changes. This is a hobby project of mine, so I might not always answer fast.
