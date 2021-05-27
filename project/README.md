# CAL - FlightNet

### Requirements

* A recent CMake version
* A C++20 compatible compiler (for example, GCC >= 10 or MSVC 2019)

### Building

    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -B build/
    cmake --build build/

The resulting executables should be placed under `build/src/`.

To build with additional data collection about the route generation process, add `-DEXTRA_ROUTE_DATA=ON` to the first CMake invocation.

### Running

The program is executed as follows:

    ./FlightNet [<names file> <locations file> <edges file>] [--planes <plane number>]

If no file paths are supplied, it defaults to `airports_full_nodes_names.txt`, `airports_full_nodes_latlng.txt` and `airports_full_edges_weights.txt`, respectively, from the current working directory.  
If `--planes` (or `-p`) isn't supplied, it will default to 10.

FlightNet writes its output to text files in its working directory, namely:

* `passengers_before.txt` contains the initial list of passengers per airport
* `passengers_after.txt` contains the final list of passengers per airport
* `strongly_connected_components.txt` contains the list of names of the strongly connected components of the produced graph
* `routeN.txt` (where N is the number of the route) contains information about one of the generated routes (this information is also written to the standard output)

---

Authors:

* Carlos Gomes (up201906622)
* Fernando Rego (up201905951)
* Joaquim Monteiro (up201905257)
