#
# This file is the sonarapp recipe.
#

SUMMARY = "Simple sonarapp application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = " \
	   file://src/common/database_msg.c \
	   file://src/common/datatype.c \
	   file://src/common/gmsg.c \
	   file://src/common/gsocket.c \
	   file://src/common/mutex.c \
	   file://src/common/rpmsg_utility.c \
	   file://src/common/service.c \
	   file://src/LSVC/l_clock.c \
	   file://src/LSVC/l_mbox.c \
	   file://src/LSVC/l_mutex.c \
	   file://src/LSVC/l_queue.c \
	   file://src/LSVC/l_sema.c \
	   file://src/LSVC/l_task.c \
	   file://src/LSVC/l_timer.c \
	   file://src/LSVC/l_utils.c \
	   file://src/h_database.c \
	   file://src/mbs_main.c \
	   file://src/t_gmsg.c \
	   file://src/t_inlinetest.c \
	   file://src/t_nvram.c \
	   file://src/t_ping.c \
	   file://src/t_slpd.c \
	   file://src/t_sudp_out.c \
	   file://src/t_system.c \
	   file://src/u_gmsg.c \
	   file://src/u_log.c \
	   file://src/u_ping.c \
	   file://Makefile \
	  "


S = "${WORKDIR}"

CFLAGS_prepend = "-I ${S}/include"


DEPENDS += " openslp"

do_compile() {
	     oe_runmake
}
FILES_${PN} = "\
	/usr/bin/sonarapp\ 
"

do_install() {
	
	install -d ${D}/usr/bin
	install -m 0755 sonarapp ${D}/usr/bin/sonarapp
}
