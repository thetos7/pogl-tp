#!/bin/sh

tar -czvf project.tar.gz src/ resources/ CMakeLists.txt setup_debug.sh setup_release.sh libs.md blender_export.md work_share.md 1>&2
