## Using std::algorithm for audio
This is a companion repository for the blog post series on using std::algorithm for audio dsp processing ([Part 1](https://www.mindmusiclabs.com/stdalgorithm-for-audio), [Part 2](https://www.mindmusiclabs.com/stdalgorithm-for-audio-part-ii/)) on using std:: It contains all the code referenced in the post and you can use it for reference or play around and see what the results are on other systems.

### Requirements
No external dependencies, though you need a compiler with C++17 support. GCC 7 or later, or Clang 5 or later should do the job.

### Building and running
The script build_and_run.sh will set up CMake with all optimisations, build and run the tests. By default it will optimise for the local system architecture.

Rebuilds can be done by calling make in the build folder.

### Build options 
The CMake variable BUFFER_SIZE can be set to build with different buffer sizes if you want to try 16, 32, 128 samples, etc.

If you want to build with a different compiler, set the CXX environment variable to point to the requested compiler, delete the build folder and run the build script again.
