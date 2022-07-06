BUILD_DIR:=build
EXPORT_NAME:=project.tar.gz
EXPORT_LIST:=\
	src/ \
	resources/ \
	CMakeLists.txt \
	Makefile \
	libs.md \
	blender_export.md \
	work_share.md \
	README.md \

CLEAN_FILES :=\
	${BUILD_DIR} \
	${EXPORT_NAME} \



default:
	@echo "run targets \`debug\` or \`release\` to choose cmake configuration, run make \`archive\` to export the project as a .tar.gz"

debug: BUILD_TYPE := Debug
debug: cmake_setup

release: BUILD_TYPE := Release
release: cmake_setup

cmake_setup:
	@rm -rf "${BUILD_DIR}"
	@cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -B "${BUILD_DIR}" 1>&2

archive:
	@tar -czvf ${EXPORT_NAME} ${EXPORT_LIST} 1>&2

clean:
	@rm -rf ${CLEAN_FILES}

.PHONY: default debug release archive cmake_setup
