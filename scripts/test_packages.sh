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

set -- podman run \
    --rm -it \
    --cap-add=ALL \
    --network=host \
    --user=root \
    --security-opt=label=disable \
    --security-opt=seccomp=unconfined \
    --security-opt=apparmor=unconfined \
    --env=DEBIAN_FRONTEND="noninteractive" \
    --env=DISPLAY="$DISPLAY" \
    --env="XDG_RUNTIME_DIR=/run/user/0" \
    --env="WAYLAND_DISPLAY=$WAYLAND_DISPLAY" \
    --volume="${XDG_RUNTIME_DIR}:/run/user/0:rw" \
    --volume="${ROOT}:${ROOT}:rw" \
    --workdir="${ROOT}"

find "$ROOT/packages" -maxdepth 1 -type f -exec sh -c '
	set -e
	package="$1"
	ext="${package##*.}"
	shift
	dir="$1"
	name="$(basename "$dir")"
	shift
	case "$ext" in
	deb)
		$@ debian:13 bash -c "$(cat << EOF
. /etc/os-release
echo "Testing on \$ID"
apt update
apt install -y binutils $package
if [ \$(ldd /opt/$name/$name | grep -c \"not\ found\") -ge 1 ]; then
	echo \"ERROR: some runtime libraries can not be resolved\"
	exit 1
fi
if ! /opt/$name/$name --help; then
	echo "ERROR: can not run executable"
	exit 1
fi
echo "Test OK!"
exit 0
EOF
)"
	;;
	rpm)
		$@ fedora:43 bash -c "$(cat << EOF
. /etc/os-release
echo "Testing on \$ID"
dnf up --refresh -y
dnf install -y binutils $package
if [ \$(ldd /opt/$name/$name | grep -c \"not\ found\") -ge 1 ]; then
	echo \"ERROR: some runtime libraries can not be resolved\"
	exit 1
fi
if ! /opt/$name/$name --help; then
	echo "ERROR: can not run executable"
	exit 1
fi
echo "Test OK!"
exit 0
EOF
)"
	;;
	*);;
	esac
' _ {} "$ROOT" "$@" \;
