# CHANGE THESE IF REQUIRED
export GCCPATH=/home/sdasgup3/Install/gcc-4.7.2.install
export SHAPE_DIR=/home/karkare/Work/ShapeAnalysis

# YOU SHULD NOT BE CHANGING THESE UNLESS ABSOLUTELY REQUIRED
export SCRIPT_DIR=${SHAPE_DIR}/Testing/RunScripts
export GCC=${GCCPATH}/bin/gcc
export CPP=${GCCPATH}/bin/g++
export PATH=${SCRIPT_DIR}:${GCCPATH}/bin:$PATH
export CPATH=/usr/include/i386-linux-gnu
#export LIBRARY_PATH=/usr/lib/i386-linux-gnu
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
export LD_LIBRARY_PATH=${GCCPATH}/install/lib/../lib64:${LD_LIBRARY_PATH}:/usr/local/lib
export GCC_VALG="${GCC} -wrapper ${SCRIPT_DIR}/scripts/valgrind_xml.sh"
export GCC_VALK="${GCC} -wrapper ${SCRIPT_DIR}/scripts/valkyrie.sh"
export SPECBMS="gzip vpr gcc mcf crafty parser perlbmk gap vortex bzip2 twolf"
