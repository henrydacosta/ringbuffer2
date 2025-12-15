# What is This?

This is a library that implements ring buffers in C:
- RingBuffer with associated functions implementing a general ring buffer; 
- RingBufferRo with associated functions implementing a read-only ring buffer; and
- RingBufferWo with associated functions implementing a writeBytes-only ring buffer.

This library does not allocate memory.
The client decides how to allocate the memory,
such as statically or dynamically,
and passes the memory to the ring buffer.

# How to Build It

To build this library, you will need:

- [CMake](https://cmake.org/), version 3.11 or higher.

- A C99 or higher compiler that CMake can use.

- Optionally, [Doxygen](https://www.doxygen.nl/index.html), if you want to build
  the docs. The CMake configuration files automatically build the docs if you
  have Doxygen installed.

- An internet connection.

This library comes with unit tests.
The unit tests do not use any external unit testing framework.

If you have all the prerequisites and want to build the library, unit tests and
documentation, you can issue the following shell commands from the directory
where you cloned this project:

<pre>
cmake -B build .
cmake --build build
</pre>

The default configuration is **Debug**. To build the **Release** version,
replace

<pre>
cmake --build build
</pre>

with

<pre>
cmake --build build <b>--config Release</b>
</pre>
