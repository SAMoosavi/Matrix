# List all the source files
set(SOURCES
        main.cpp
)

# List all the header files
set(HEADERS
        concept.h
        matrix.h
)

# List all the inline header files
set(INLINE_HEADERS
)

# List all the temporary header files
set(TEMP_HEADERS
        matrix-tmp.h
)

# Add the executable
add_executable(Matrix ${SOURCES} ${HEADERS} ${INLINE_HEADERS} ${TEMP_HEADERS})
