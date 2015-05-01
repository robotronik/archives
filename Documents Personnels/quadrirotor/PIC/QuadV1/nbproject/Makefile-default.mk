#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/QuadV1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/QuadV1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c i2c.c i2cIMU.c IMU.c dmp.c mathHelper.c init.c debug.c asser.c motors.c spi.c Mirf.c "DEE Emulation 16-bit.c" "Flash Operations.s"

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/i2c.o ${OBJECTDIR}/i2cIMU.o ${OBJECTDIR}/IMU.o ${OBJECTDIR}/dmp.o ${OBJECTDIR}/mathHelper.o ${OBJECTDIR}/init.o ${OBJECTDIR}/debug.o ${OBJECTDIR}/asser.o ${OBJECTDIR}/motors.o ${OBJECTDIR}/spi.o ${OBJECTDIR}/Mirf.o "${OBJECTDIR}/DEE Emulation 16-bit.o" "${OBJECTDIR}/Flash Operations.o"
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/i2c.o.d ${OBJECTDIR}/i2cIMU.o.d ${OBJECTDIR}/IMU.o.d ${OBJECTDIR}/dmp.o.d ${OBJECTDIR}/mathHelper.o.d ${OBJECTDIR}/init.o.d ${OBJECTDIR}/debug.o.d ${OBJECTDIR}/asser.o.d ${OBJECTDIR}/motors.o.d ${OBJECTDIR}/spi.o.d ${OBJECTDIR}/Mirf.o.d "${OBJECTDIR}/DEE Emulation 16-bit.o.d" "${OBJECTDIR}/Flash Operations.o.d"

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/i2c.o ${OBJECTDIR}/i2cIMU.o ${OBJECTDIR}/IMU.o ${OBJECTDIR}/dmp.o ${OBJECTDIR}/mathHelper.o ${OBJECTDIR}/init.o ${OBJECTDIR}/debug.o ${OBJECTDIR}/asser.o ${OBJECTDIR}/motors.o ${OBJECTDIR}/spi.o ${OBJECTDIR}/Mirf.o ${OBJECTDIR}/DEE\ Emulation\ 16-bit.o ${OBJECTDIR}/Flash\ Operations.o

# Source Files
SOURCEFILES=main.c i2c.c i2cIMU.c IMU.c dmp.c mathHelper.c init.c debug.c asser.c motors.c spi.c Mirf.c DEE Emulation 16-bit.c Flash Operations.s


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/QuadV1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ128MC802
MP_LINKER_FILE_OPTION=,--script=p33FJ128MC802.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2c.o: i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	@${RM} ${OBJECTDIR}/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c.c  -o ${OBJECTDIR}/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2cIMU.o: i2cIMU.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/i2cIMU.o.d 
	@${RM} ${OBJECTDIR}/i2cIMU.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2cIMU.c  -o ${OBJECTDIR}/i2cIMU.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2cIMU.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/i2cIMU.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/IMU.o: IMU.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/IMU.o.d 
	@${RM} ${OBJECTDIR}/IMU.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IMU.c  -o ${OBJECTDIR}/IMU.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/IMU.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/IMU.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dmp.o: dmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/dmp.o.d 
	@${RM} ${OBJECTDIR}/dmp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dmp.c  -o ${OBJECTDIR}/dmp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dmp.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/dmp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mathHelper.o: mathHelper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/mathHelper.o.d 
	@${RM} ${OBJECTDIR}/mathHelper.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mathHelper.c  -o ${OBJECTDIR}/mathHelper.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mathHelper.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/mathHelper.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/init.o: init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/init.o.d 
	@${RM} ${OBJECTDIR}/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  init.c  -o ${OBJECTDIR}/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/init.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/debug.o: debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/debug.o.d 
	@${RM} ${OBJECTDIR}/debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  debug.c  -o ${OBJECTDIR}/debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/debug.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/asser.o: asser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/asser.o.d 
	@${RM} ${OBJECTDIR}/asser.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  asser.c  -o ${OBJECTDIR}/asser.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/asser.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/asser.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/motors.o: motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/motors.o.d 
	@${RM} ${OBJECTDIR}/motors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  motors.c  -o ${OBJECTDIR}/motors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/motors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/motors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/spi.o: spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/spi.o.d 
	@${RM} ${OBJECTDIR}/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi.c  -o ${OBJECTDIR}/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/spi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Mirf.o: Mirf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Mirf.o.d 
	@${RM} ${OBJECTDIR}/Mirf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mirf.c  -o ${OBJECTDIR}/Mirf.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Mirf.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/Mirf.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/DEE\ Emulation\ 16-bit.o: DEE\ Emulation\ 16-bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/DEE\ Emulation\ 16-bit.o.d 
	@${RM} "${OBJECTDIR}/DEE Emulation 16-bit.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "DEE Emulation 16-bit.c"  -o "${OBJECTDIR}/DEE Emulation 16-bit.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DEE Emulation 16-bit.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/DEE Emulation 16-bit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2c.o: i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	@${RM} ${OBJECTDIR}/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c.c  -o ${OBJECTDIR}/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2c.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2cIMU.o: i2cIMU.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/i2cIMU.o.d 
	@${RM} ${OBJECTDIR}/i2cIMU.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2cIMU.c  -o ${OBJECTDIR}/i2cIMU.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2cIMU.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/i2cIMU.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/IMU.o: IMU.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/IMU.o.d 
	@${RM} ${OBJECTDIR}/IMU.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IMU.c  -o ${OBJECTDIR}/IMU.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/IMU.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/IMU.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dmp.o: dmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/dmp.o.d 
	@${RM} ${OBJECTDIR}/dmp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dmp.c  -o ${OBJECTDIR}/dmp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dmp.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/dmp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mathHelper.o: mathHelper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/mathHelper.o.d 
	@${RM} ${OBJECTDIR}/mathHelper.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mathHelper.c  -o ${OBJECTDIR}/mathHelper.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mathHelper.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/mathHelper.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/init.o: init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/init.o.d 
	@${RM} ${OBJECTDIR}/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  init.c  -o ${OBJECTDIR}/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/init.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/debug.o: debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/debug.o.d 
	@${RM} ${OBJECTDIR}/debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  debug.c  -o ${OBJECTDIR}/debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/debug.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/asser.o: asser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/asser.o.d 
	@${RM} ${OBJECTDIR}/asser.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  asser.c  -o ${OBJECTDIR}/asser.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/asser.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/asser.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/motors.o: motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/motors.o.d 
	@${RM} ${OBJECTDIR}/motors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  motors.c  -o ${OBJECTDIR}/motors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/motors.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/motors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/spi.o: spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/spi.o.d 
	@${RM} ${OBJECTDIR}/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi.c  -o ${OBJECTDIR}/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/spi.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Mirf.o: Mirf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Mirf.o.d 
	@${RM} ${OBJECTDIR}/Mirf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mirf.c  -o ${OBJECTDIR}/Mirf.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Mirf.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/Mirf.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/DEE\ Emulation\ 16-bit.o: DEE\ Emulation\ 16-bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/DEE\ Emulation\ 16-bit.o.d 
	@${RM} "${OBJECTDIR}/DEE Emulation 16-bit.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "DEE Emulation 16-bit.c"  -o "${OBJECTDIR}/DEE Emulation 16-bit.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DEE Emulation 16-bit.o.d"      -g -omf=elf -O0 -msmart-io=1 -Werror -Wall -msfr-warn=on
	@${FIXDEPS} "${OBJECTDIR}/DEE Emulation 16-bit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Flash\ Operations.o: Flash\ Operations.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Flash\ Operations.o.d 
	@${RM} "${OBJECTDIR}/Flash Operations.o" 
	${MP_CC} $(MP_EXTRA_AS_PRE)  "Flash Operations.s"  -o "${OBJECTDIR}/Flash Operations.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -Wa,-MD,"${OBJECTDIR}/Flash Operations.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/Flash Operations.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/Flash\ Operations.o: Flash\ Operations.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Flash\ Operations.o.d 
	@${RM} "${OBJECTDIR}/Flash Operations.o" 
	${MP_CC} $(MP_EXTRA_AS_PRE)  "Flash Operations.s"  -o "${OBJECTDIR}/Flash Operations.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -Wa,-MD,"${OBJECTDIR}/Flash Operations.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/Flash Operations.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/QuadV1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/QuadV1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=4096,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/QuadV1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/QuadV1.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=4096,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/QuadV1.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
