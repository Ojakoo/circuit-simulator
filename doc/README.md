## Compiling and running the program

- You can find basic user guide in the cpp_project_documentation.pdf file!

At first the user must install the needed packages and submodules needed for the program. If using Linux, the needed packages can be installed running following commands in terminal:

```
sudo apt-get install build-essential
sudo apt install gcc
sudo apt install g++
sudo apt install make
sudo snap install cmake --classic
sudo apt-get install libx11-dev
sudo apt-get install xorg-dev
sudo apt-get install freeglut3-dev
sudo apt-get install libudev-dev
```

After downloading the package from git the user must install the submodules needed for the program. This means downloading Eigen which is a matrix manipulation library. This can be done by running following commands in terminal:

```
cd circuit-sim-2020-2
git submodule update --init --recursive
```

After downloading and installing the needed submodules the user can build and run the programming executing following commands in terminal:

```
cd circuit-sim-2020-2
mkdir build
cd build
cmake ..
make
./src/main
```

We also implemented a couple of automated tests. Tests can be run by typing the following commands into the terminal. 

```
cd circuit-sim-2020-2
mkdir build
cd build
cmake ..
make
make test
```

The user can also remove everything from the build folder by executing one of the following commands.

```
rm -r build
rm -rf build (incase of error: sfml is write protected)
```