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
CC=clang
CCC=clang++
CXX=clang++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=CLang-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CrystalSystems_a.o \
	${OBJECTDIR}/CrystalSystems_c.o \
	${OBJECTDIR}/CrystalSystems_h.o \
	${OBJECTDIR}/CrystalSystems_m.o \
	${OBJECTDIR}/CrystalSystems_o.o \
	${OBJECTDIR}/CrystalSystems_t.o \
	${OBJECTDIR}/Eqiv.o \
	${OBJECTDIR}/Parser.o \
	${OBJECTDIR}/ResParser.o \
	${OBJECTDIR}/Restraint.o \
	${OBJECTDIR}/ShelxState.o \
	${OBJECTDIR}/SymOp.o \
	${OBJECTDIR}/XYZ.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/myOpt.o \
	${OBJECTDIR}/usage.o \
	${OBJECTDIR}/xAtom.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cellopt

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cellopt: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cellopt ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/CrystalSystems_a.o: CrystalSystems_a.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CrystalSystems_a.o CrystalSystems_a.cpp

${OBJECTDIR}/CrystalSystems_c.o: CrystalSystems_c.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CrystalSystems_c.o CrystalSystems_c.cpp

${OBJECTDIR}/CrystalSystems_h.o: CrystalSystems_h.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CrystalSystems_h.o CrystalSystems_h.cpp

${OBJECTDIR}/CrystalSystems_m.o: CrystalSystems_m.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CrystalSystems_m.o CrystalSystems_m.cpp

${OBJECTDIR}/CrystalSystems_o.o: CrystalSystems_o.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CrystalSystems_o.o CrystalSystems_o.cpp

${OBJECTDIR}/CrystalSystems_t.o: CrystalSystems_t.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CrystalSystems_t.o CrystalSystems_t.cpp

${OBJECTDIR}/Eqiv.o: Eqiv.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Eqiv.o Eqiv.cpp

${OBJECTDIR}/Parser.o: Parser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parser.o Parser.cpp

${OBJECTDIR}/ResParser.o: ResParser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ResParser.o ResParser.cpp

${OBJECTDIR}/Restraint.o: Restraint.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Restraint.o Restraint.cpp

${OBJECTDIR}/ShelxState.o: ShelxState.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ShelxState.o ShelxState.cpp

${OBJECTDIR}/SymOp.o: SymOp.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SymOp.o SymOp.cpp

${OBJECTDIR}/XYZ.o: XYZ.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/XYZ.o XYZ.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/myOpt.o: myOpt.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myOpt.o myOpt.cpp

${OBJECTDIR}/usage.o: usage.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/usage.o usage.cpp

${OBJECTDIR}/xAtom.o: xAtom.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/xAtom.o xAtom.cpp

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
