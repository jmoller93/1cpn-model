CC =     g++
BINDIR = '../../bin'
LIBRY      =  -L/usr/local/lib -lm 
INCLD      = -I../include 

DEBUG ?= 0
ifeq ($(DEBUG),1)
	CFLGS =  -ggdb -Wall -std=c++11
else
	CFLGS =  -O2 -finline-functions -funroll-loops -std=c++11
endif

all: calc_fvu_angle calc_site_dist shuffle_repx_dump calc_lp calc_sedimentation\
     calc_msd_com calc_com clip_traj analyze_two_nucl_config calc_gyration_tensor\
	 calc_nucl_angles calc_contact_nucls calc_rouse_modes calc_restart

calc_lp : calc_lp.cpp 
	${CC} ${CFLGS} ${INCLD} calc_lp.cpp -o  ${BINDIR}/calc_lp.exe

calc_fvu_angle : calc_fvu_angle.cpp 
	${CC} ${CFLGS} ${INCLD} calc_fvu_angle.cpp -o  ${BINDIR}/calc_fvu_angle.exe

calc_site_dist : calc_site_dist.cpp
	${CC} ${CFLGS} ${INCLD} calc_site_dist.cpp -o  ${BINDIR}/calc_site_dist.exe

shuffle_repx_dump : shuffle_repx_dump.cpp
	${CC} ${CFLGS} ${INCLD} shuffle_repx_dump.cpp -o  ${BINDIR}/shuffle_repx_dump.exe

calc_sedimentation : calc_sedimentation.cpp
	${CC} ${CFLGS} ${INCLD} calc_sedimentation.cpp -o  ${BINDIR}/calc_sedimentation.exe

calc_msd_com : calc_msd_com.cpp 
	${CC} ${CFLGS} ${INCLD} calc_msd_com.cpp -o  ${BINDIR}/calc_msd_com.exe

calc_com : calc_com.cpp 
	${CC} ${CFLGS} ${INCLD} calc_com.cpp -o  ${BINDIR}/calc_com.exe

calc_gyration_tensor : calc_gyration_tensor.cpp
	${CC} ${CFLGS} ${INCLD} calc_gyration_tensor.cpp -o  ${BINDIR}/calc_gyration_tensor.exe

calc_nucl_angles : calc_nucl_angles.cpp
	${CC} ${CFLGS} ${INCLD} calc_nucl_angles.cpp -o  ${BINDIR}/calc_nucl_angles.exe

clip_traj : clip_traj.cpp 
	${CC} ${CFLGS} ${INCLD} clip_traj.cpp -o  ${BINDIR}/clip_traj.exe

calc_contact_nucls : calc_contact_nucls.cpp 
	${CC} ${CFLGS} ${INCLD} calc_contact_nucls.cpp -o  ${BINDIR}/calc_contact_nucls.exe

calc_restart : calc_restart.cpp
	${CC} ${CFLGS} ${INCLD} calc_restart.cpp -o  ${BINDIR}/calc_restart.exe

calc_rouse_modes : calc_rouse_modes.cpp
	${CC} ${CFLGS} ${INCLD} calc_rouse_modes.cpp -o  ${BINDIR}/calc_rouse_modes.exe

analyze_two_nucl_config : analyze_two_nucl_config.cpp 
	${CC} ${CFLGS} ${INCLD} analyze_two_nucl_config.cpp -o  ${BINDIR}/analyze_two_nucl_config.exe
