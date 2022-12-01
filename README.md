# OpenGL game
A 3D game for education purposes on OpenGL.

This follows [this
tutorial](https://medium.com/geekculture/a-beginners-guide-to-setup-opengl-in-linux-debian-2bfe02ccd1e)
and requires [glfw](https://github.com/glfw/glfw) to compile.

Install these if the program is not compiling:

```
sudo apt-get install cmake pkg-config
sudo apt-get install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev
sudo apt-get install libglew-dev libglfw3-dev libglm-dev
sudo apt-get install libao-dev libmpg123-dev
```

# Installing glfw

We need this to setup the OpenGL window more easily

Install the `glfw` directory by doing:

```
cd glfw
cmake .
make
sudo make install
```

# Installing glm

We use this for matrix operations

Install `glm` through the following commands

```
cd glm
cmake .
make
sudo make install
```
