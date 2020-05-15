FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
FILESPATH_prepend := "/home/hasitha/MegaLive/MegaLiveProj/components/plnx_workspace/sources/linux-xlnx/oe-local-files:"
# srctreebase: /home/hasitha/MegaLive/MegaLiveProj/components/plnx_workspace/sources/linux-xlnx

inherit externalsrc
# NOTE: We use pn- overrides here to avoid affecting multiple variants in the case where the recipe uses BBCLASSEXTEND
EXTERNALSRC_pn-linux-xlnx = "/home/hasitha/MegaLive/MegaLiveProj/components/plnx_workspace/sources/linux-xlnx"
SRCTREECOVEREDTASKS = "do_validate_branches do_kernel_checkout do_fetch do_unpack do_kernel_configme do_kernel_configcheck"

do_patch() {
    :
}

do_configure_append() {
    cp ${B}/.config ${S}/.config.baseline
    ln -sfT ${B}/.config ${S}/.config.new
}

# initial_rev: b983d5fd71d4feaf494cdbe0593ecc29ed471cb8
