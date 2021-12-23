# CasualGame

SFML Raycasting Game/Engine work in progress

Current Features:
* textured raycasting renderer
* minimap
* player controls
* level editor

<img src="screen.png" />

## Getting started

Download SFML SDK from here:

https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip

Download GLEW from here:

https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download

Create a new folder in ```CasualGame``` called ```libraries```.
Unzip both zip files into the newly created ```CasualGame/libraries``` folder.

Launch the solution in Visual Studio 2019. 

Build should now work.

To run the build these libraries need to be copied:

- Copy the *.dll files* from SFML and GLEW bin/ folders to the folder with CasualGame.exe (x64/Debug or x64/Release)
- Copy the *CasualGame/resources* folder to the folder with CasualGame.exe (x64/Debug or x64/Release)
