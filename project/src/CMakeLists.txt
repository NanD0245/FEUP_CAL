add_executable(
    FlightNet
    airport.cpp
    airport.h
    airport_set.cpp
    airport_set.h
    graph.cpp
    graph.h
    import.cpp
    import.h
    main.cpp
    plane.cpp
    plane.h
    route.cpp
    route.h
    util.cpp
    util.h
)
target_link_libraries(FlightNet PRIVATE project_options project_warnings)

option(EXTRA_ROUTE_DATA "Enable additional data collection about the route generation process" OFF)
if(EXTRA_ROUTE_DATA)
    target_compile_definitions(FlightNet PUBLIC ENABLE_EXTRA_ROUTE_DATA_COLLECTION)
endif()

configure_file(../example_data/airports_full_edges.txt airports_full_edges.txt COPYONLY)
configure_file(../example_data/airports_full_edges_weights.txt airports_full_edges_weights.txt COPYONLY)
configure_file(../example_data/airports_full_nodes_latlng.txt airports_full_nodes_latlng.txt COPYONLY)
configure_file(../example_data/airports_full_nodes_names.txt airports_full_nodes_names.txt COPYONLY)
