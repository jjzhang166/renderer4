# OpenGL Renderer

Messing around with OpenGL. Uses GLFW3 and GLAD. Later, maybe Assimp and GLM.

Latest output:
![current screenshot](https://raw.githubusercontent.com/stevensona/renderer/master/screenshot.png)


# Linux preparation steps

```
sudo apt install libgl1-mesa-dev libglu1-mesa-dev libvulkan-dev xorg-dev
git clone git@github.com:glfw/glfw.git
cd glfw
cmake .
make
sudo make install
sudo cp -r include/GLFW /usr/include/
```

# Compiling
```
cmake .
make
```
