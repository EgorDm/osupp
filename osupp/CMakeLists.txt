project(osupp C CXX)

file(GLOB SOURCE_FILES
        "*.h"
        "*.cpp"
        "*.hpp"
        )

add_library(${PROJECT_NAME} ${SOURCE_FILES})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)