
cmake_minimum_required(VERSION 3.20)
project(TransazioniFinanziarie)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

# Executable principal
add_executable(TransazioniFinanziarie
        main.cpp
)

# Executable de test (fichier dans le dossier racine)
add_executable(test_transazioni
        test_transazioni.cpp
)
target_link_libraries(test_transazioni PRIVATE Catch2::Catch2WithMain)
