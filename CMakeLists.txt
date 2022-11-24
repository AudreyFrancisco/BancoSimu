#----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Banco)

#----------------------------------------------------------------------------
find_package(Geant4 REQUIRED ui_all vis_all)

#----------------------------------------------------------------------------
include(${Geant4_USE_FILE})

       
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(sim sim.cc ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
add_custom_target(Banco DEPENDS sim)
