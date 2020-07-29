# Sandbox

Sandbox is 3D game engine. It's my first experience in computer graphics so far.

![](https://i.ibb.co/VB1Kz9s/2.png)

## Idea
Here I want to create a small game engine, with different functional parts, and with different graphics APIs in future. So this is my first attempt to fit these ideas in one project.
In this project I also would like to find out a good balance between flexibility and performance in graphics application.

## Features
* Loading gltf2 models using tinygltflib (no animations supported)
* Render models using Opengl API
* PBR/IBL
* Some weird architecture

## Screenshots
![](https://i.ibb.co/7Q9NVQT/13-1.png)  
[Link](https://sketchfab.com/3d-models/material-ball-in-3d-coat-a6bdf1d11d714e07b9dd99dda02de965) to original model. 
![](https://i.ibb.co/bjVgKkN/Tank.png)  
[Link](https://sketchfab.com/3d-models/kv-2-heavy-tank-1940-ba8b84d78c0a42038cf2eaa4210ef296) to original model. 
![](https://i.ibb.co/b7Ys6Nx/10-3.png)

## How to build
#### Windows, VS
```
$ CMake -G "Visual Studio 16 2019"
```