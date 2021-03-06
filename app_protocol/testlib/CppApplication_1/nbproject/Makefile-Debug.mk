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
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=
AS=as.exe

# Macros
CND_PLATFORM=MinGW_QT-Windows
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/_ext/43898991/lbproto.o \
	${OBJECTDIR}/_ext/43898991/sbuilder.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/MinGW_QT-Windows/cppapplication_1.exe

dist/Debug/MinGW_QT-Windows/cppapplication_1.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/MinGW_QT-Windows
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cppapplication_1 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I../.. -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/_ext/43898991/lbproto.o: ../../lbproto.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	$(COMPILE.cc) -g -I../.. -o ${OBJECTDIR}/_ext/43898991/lbproto.o ../../lbproto.cpp

${OBJECTDIR}/_ext/43898991/sbuilder.o: ../../sbuilder.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	$(COMPILE.cc) -g -I../.. -o ${OBJECTDIR}/_ext/43898991/sbuilder.o ../../sbuilder.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/MinGW_QT-Windows/cppapplication_1.exe

# Subprojects
.clean-subprojects:
