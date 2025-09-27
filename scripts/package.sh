#!/bin/sh

set -e

HERE="$(dirname "$(realpath "$0")")"
ROOT="$(realpath "$HERE/..")"
if [ "$ROOT" != "$PWD" ]; then
	cd "$ROOT" || exit 1
fi
ROOT="$(pwd)"

if [ -f "$ROOT/requirements.txt" ]; then
	if [ ! -f "$ROOT/.venv/bin/activate" ]; then
		python3 -m venv "$ROOT/.venv"
		# shellcheck source=/dev/null
		. "$ROOT/.venv/bin/activate"
		pip install -U pip
		pip install -r "$ROOT/requirements.txt"
	else
		# shellcheck source=/dev/null
		. "$ROOT/.venv/bin/activate"
	fi
fi

PRESET="release"
GENERATOR="TGZ;DEB;RPM"
STANDALONE="OFF"
BUILD_EXAMPLES="OFF"
TEST_PACKAGES=false
while getopts ":g:dpet" OPT; do
	case $OPT in
	d)
		PRESET="debug"
		;;
	g)
		GENERATOR="$OPTARG"
		;;
	p)
		STANDALONE="ON"
		;;
	e)
		BUILD_EXAMPLES="ON"
		;;
	t)
		TEST_PACKAGES=true
		;;
	*)
		echo "error: unrecognized option $OPT"
		echo "usage: ./scripts/package.sh -g DEB;RPM [-r] [-p] [-e]"
		;;
	esac
done

if [ -z "$GENERATOR" ]; then
	echo "error: generator must be non empty"
	echo "usage: ./scripts/package.sh -g DEB;RPM [-r] [-p] [-e]"
	exit 1
fi

rm -rf "$ROOT/build" || true
rm -rf "$ROOT/install" || true
rm -rf "$ROOT/packages" || true

echo "executing in $ROOT"
CONFIGURE_OPTIONS="-DDEPOT_STANDALONE_PACKAGE=$STANDALONE -DDEPOT_BUILD_EXAMPLES=$BUILD_EXAMPLES"
CONFIGURE_OPTIONS="$CONFIGURE_OPTIONS -DCMAKE_INSTALL_PREFIX=$ROOT/install"

echo "Re-configuring project with preset $PRESET"
cmake --preset $PRESET $CONFIGURE_OPTIONS --fresh

echo "Build project with preset $PRESET"
cmake --build --preset $PRESET --parallel "$(nproc)"

echo "Packaging project with preset $PRESET and generator $GENERATOR"
LOG_FILE="/tmp/intraop.log"
cpack --config "$ROOT/build/$PRESET/CPackConfig.cmake" -G "$GENERATOR" | tee "$LOG_FILE"

if $TEST_PACKAGES; then
	"$ROOT/scripts/test_packages.sh"
fi
