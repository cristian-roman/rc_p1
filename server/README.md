====Development Guidelines====

As a convention, while creating new libraries:

The folder names for new libraries are not required to be the same as the library name.
Don't forget to add the names and paths of new libraries to:

The libraries array in the "CMakeLists" script.
The paths array in the "CMakeLists" script.

In order for json parser to work please install it with the following command:
sudo apt install libjansson-dev