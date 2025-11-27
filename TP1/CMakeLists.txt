cmake_minimum_required(VERSION 3.10)
project(Systemes_avances_TP1 LANGUAGES C)

# Standard C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Dossier de sortie pour les exécutables (relatif à ce CMakeLists.txt)
set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(MAKE_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Exécutables indépendants
add_executable(exo1 src/exo1.c)
add_executable(exo2 src/exo2.c)
add_executable(exo3 src/exo3.c)