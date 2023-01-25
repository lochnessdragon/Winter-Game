# Game Engine
A small game engine

## Dependency Management
 The cmake script will first check if the package is intalled on the system.
 If it is not, then it will check if a source directory for the package has been installed.
 If that is not true and INSTALL_DEPS=1, then the script will download the required project and build the sources.