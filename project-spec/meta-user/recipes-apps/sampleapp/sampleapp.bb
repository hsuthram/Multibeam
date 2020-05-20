#
# This file is the sampleapp recipe.
#

SUMMARY = "Simple sampleapp application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://sampleapp.c \
	   file://common/database_msg.c \
	   file://common/database_msg.h \
	   file://common/datatype.c \
	   file://common/gmsg.c \
	   file://common/gmsg.h \
	   file://common/gmsg_info.h \
	   file://common/gmsg_id.h \
	   file://common/gsocket.c \
	   file://common/gmsg_req.h \
	   file://common/gsocket.h \
	   file://common/gmsg_addr.h \
	   file://common/mutex.c \
	   file://common/mutex.h \
	   file://common/rpmsg_utility.c \
	   file://common/rpmsg_utility.h \
	   file://common/service.c \
	   file://common/service.h \
	   file://LSVC/rtx2lsvc.h \
	   file://LSVC/l_clock.c \
	   file://LSVC/linuxsvc.h \
	   file://lsvc.h \
	   file://LSVC/l_clock.h \
	   file://LSVC/l_mbox.c \
	   file://LSVC/l_mbox.h \
	   file://LSVC/l_mutex.c \
	   file://LSVC/l_mutex.h \
	   file://LSVC/l_queue.c \
	   file://LSVC/l_queue.h \
	   file://LSVC/l_sema.c \
	   file://LSVC/l_sema.h \
	   file://LSVC/l_task.c \
	   file://LSVC/l_task.h \
	   file://LSVC/l_timer.c \
	   file://LSVC/l_timer.h \
	   file://LSVC/l_utils.c \
	   file://LSVC/l_utils.h\
	   file://h_database.c \
	   file://h_database.h \
	   file://mbs_main.c \
           file://global.h \
	   file://common/datatype.h \
	   file://types.h \
	   file://t_gmsg.c \
	   file://t_gmsg.h \
	   file://t_inlinetest.c \
	   file://t_nvram.c \
	   file://t_ping.c \
	   file://t_slpd.c \
	   file://t_sudp_out.c \
	   file://t_sudp_out.h \
	   file://t_system.c \
	   file://u_gmsg.c \
	   file://u_gmsg.h \
	   file://u_log.c \
	   file://u_log.h \
	   file://u_ping.c \
	   file://u_ping.h \
	   file://fcid.h \
file://lsvc.c \
	   file://message.h \
	   file://datadict.h \
	   file://h_ping.h \
	   file://s_sonar.h \
	   file://Makefile \
		  "

S = "${WORKDIR}"

DEPENDS += "openslp"


CFLAGS_append += "-I ${S}/common"
CFLAGS_append += "-I ${S}/LSVC"
CFLAGS_prepend += "-I ${S}/"
#EXTRA_OEMAKE = "'CC=${CC}' CFLAGS=${CFLAGS} -I${S}/include -DWITHOUT_XATTR' 'BUILDDIR=${S}'"

do_compile() {
    oe_runmake
}


FILES_${PN} = "\
	/usr/bin/sampleapp\ 
"

do_install() {
        install -d ${D}/usr/bin
	install -m 0755 sampleapp ${D}/usr/bin/sampleapp
}
