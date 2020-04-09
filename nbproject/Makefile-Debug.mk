#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/IndexBuffer.o \
	${OBJECTDIR}/src/Renderer.o \
	${OBJECTDIR}/src/Shader.o \
	${OBJECTDIR}/src/VertexArray.o \
	${OBJECTDIR}/src/VertexBuffer.o \
	${OBJECTDIR}/src/VertexBufferLayout.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/vendor/stb/stb_image.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-ggdb
CXXFLAGS=-ggdb

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=lib/libglfw3.a lib/libGLEW.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/myopengl

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/myopengl: lib/libglfw3.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/myopengl: lib/libGLEW.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/myopengl: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/myopengl ${OBJECTFILES} ${LDLIBSOPTIONS} -framework CoreFoundation -framework CoreGraphics -framework IOKit -framework AppKit -framework OpenGL

${OBJECTDIR}/src/IndexBuffer.o: src/IndexBuffer.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/include -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/IndexBuffer.o src/IndexBuffer.cpp

${OBJECTDIR}/src/Renderer.o: src/Renderer.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/include -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Renderer.o src/Renderer.cpp

${OBJECTDIR}/src/Shader.o: src/Shader.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/include -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Shader.o src/Shader.cpp

${OBJECTDIR}/src/VertexArray.o: src/VertexArray.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/include -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/VertexArray.o src/VertexArray.cpp

${OBJECTDIR}/src/VertexBuffer.o: src/VertexBuffer.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/include -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/VertexBuffer.o src/VertexBuffer.cpp

${OBJECTDIR}/src/VertexBufferLayout.o: src/VertexBufferLayout.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/include -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/VertexBufferLayout.o src/VertexBufferLayout.cpp

${OBJECTDIR}/src/main.o: src/main.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/include -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/vendor/stb/stb_image.o: src/vendor/stb/stb_image.cpp
	${MKDIR} -p ${OBJECTDIR}/src/vendor/stb
	${RM} "$@.d"
	$(COMPILE.cc) -g -Ilib/include -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vendor/stb/stb_image.o src/vendor/stb/stb_image.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
