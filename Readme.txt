Pixel Intensity Comparison Face Detection

Originally from: https://github.com/nenadmarkus/pico.git

Modified to use OpenMP to detect multiple rotations concurrently.

Also contains an app in RotfaceDtree folder in Java to parse the model data array. The generated code is currently not used though as is doesn’t seem to be any faster than directly using the data array.
