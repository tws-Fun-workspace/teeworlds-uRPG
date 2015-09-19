#!/bin/sh

reexecd=false
if [ "x$1" '=' 'x-u' ]; then
	reexecd=true
	shift
fi

forceflag='--yes-i-actually-want-to-run-this-as-root-even-though-i-am-perfectly-aware-of-what-a-stupid-and-dangerous-idea-it-is'

allowroot=false
if [ "x$1" '=' "x$forceflag" ]; then
	allowroot=true
	shift
fi


binary='openfng_srv'

argv=
bam='.bam_repo/bam/bam'
git="$(command -v git)"
prgnam="$(basename "$0")"

Main()
{
	argv="$(list_mklist "$@")"

	if [ $(id -u) -eq 0 ]; then
		$allowroot || Bomb "Refusing to run as root.  You can override this check with the $forceflag command-line option, but you probably shouldn't." silent
	fi

	Deps
	Update

	[ -x "$binary" ] || BuildTW

	Run
	return $?
}

Say()
{
	printf '%s: %s\n' "$prgnam" "$1" >&2
}

Bomb()
{
	printf '%s: ERROR: %s\n' "$prgnam" "$1" >&2
	if [ -z "$2" ]; then
		printf '%s: ************************************************\n' "$prgnam" >&2
		printf '%s: * !!!       PLEASE REPORT THIS ERROR       !!! *\n' "$prgnam" >&2
		printf '%s: * !!!  Write a mail to van.fstd@gmail.com  !!! *\n' "$prgnam" >&2
		printf '%s: * !!!  or report it on github if you have  !!! *\n' "$prgnam" >&2
		printf '%s: * !!!  a github account:                   !!! *\n' "$prgnam" >&2
		printf '%s: * https://github.com/fstd/teeworlds/issues/new *\n' "$prgnam" >&2
		printf '%s: ************************************************\n' "$prgnam" >&2
	fi
	exit 1
}

Update()
{
	tmp=$(mktemp /tmp/openfng.sh.XXXXXXXX)
	"$git" log -n1 >$tmp
	Say "Pulling new commits, if any"
	"$git" pull
	if [ $("$git" log -n1 | diff -u $tmp - | wc -l) -gt 0 ]; then
		# move away binary if we updated successfully so that it gets rebuilt
		mv "$binary" "${binary}.old"
		Say 'Updated'

		$reexecd && Bomb 'Two updates in a row? Something is wrong.'

		Say 'Re-executing script in case it was updated too'
		eval "set -- $argv"
		exec /bin/sh "$0" -u "$@"
	else
		Say "Up to date."
	fi
	rm "$tmp"
}

Deps()
{
	Say "Checking whether we have a toolchain..."
	command -v cc >/dev/null \
	    || command -v gcc >/dev/null \
	    || command -v clang >/dev/null \
	    || Bomb "Please install the toolchain (try 'sudo apt-get install build-essential')" silent

	Say "Checking for git..."
	[ -n "$git" ] || Bomb "Please install 'git' (try 'sudo apt-get install git')" silent

	Say "Checking for bam..."
	[ -x "$bam" ] || BuildBam
}

BuildBam()
{
	Say "We need to build bam."
	mkdir -p .bam_repo
	if ! grep -Fq .bam_repo .git/info/exclude; then
		echo '.bam_repo' >> .git/info/exclude
	fi
	cd .bam_repo || Bomb "Failed to cd into .bam_repo"
	Say "Cloning bam"
	"$git" clone http://github.com/fstd/bam.git || Bomb "Failed to clone the bam repo"
	cd bam || Bomb "Could not cd into .bam_repo/bam"

	Say "Compiling bam"
	./make_unix.sh
	[ -x 'bam' ] || Bomb "Could not build bam"
	bam="$(pwd)/bam"
	cd ../..
	Say "Bam successfully built"
}

BuildTW()
{
	Say "Building OpenFNG"
	"$bam" config
	"$bam" -c all #clean just to be sure
	"$bam" server_release

	[ -x "$binary" ] || Bomb "Failed to build OpenFNG"
	Say "OpenFNG successfully built"
}

Run()
{
	Say "Running OpenFNG"
	eval "set -- $argv"
	./$binary "$@"
	Say "OpenFNG terminated"
	return $?
}

# ----This essentially is lstd-tiny.inc.sh (http://github.com/fstd/lstd) -------
_lstd_tn_esc()
{
	_lstd_tn_input="$1"

	printf "'"
	while :; do
		case "$_lstd_tn_input" in
			\'*) printf "'\\\\''"              ;; # '\''
			 ?*) printf '%c' "$_lstd_tn_input" ;;
			 "") break                         ;;
		esac
		_lstd_tn_input="${_lstd_tn_input#?}"
	done
	printf "'"
}

list_mklist()
{
	while [ $# -gt 0 ]; do
		_lstd_tn_esc "$1"
		printf ' '
		shift
	done
}
# ---------------------------End of lstd-tiny.inc.sh ---------------------------

Main "$@"
exit $?
