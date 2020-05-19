#
# This file is the echotesths recipe.
#

SUMMARY = "Simple echotesths application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "\
	file://LICENSE \
	file://Makefile \
	file://echotesths.c \
	file://rpmsg_utility.c\
	file://sharememechotest.c\
	file://jo_MBc2.elf\
	"

S = "${WORKDIR}"
CFLAGS_prepend = "-I ${S}/include"
RRECOMMENDS_${PN} = "kernel-module-rpmsg-char"

FILES_${PN} = "\
	/usr/bin/echotesths\ 
       /lib/firmware/jo_MBc2.elf\
"


do_install () {

     	# Install firmware into /lib/firmware on target   
      	install -d ${D}/lib/firmware   
      	install -m 0644 ${S}/jo_MBc2.elf ${D}/lib/firmware/jo_MBc2.elf

	install -d ${D}/usr/bin
	install -m 0755 echotesths ${D}/usr/bin/echotesths
}
