#!/bin/sh

# exit on error
set -e

# trap the exit status
trap "exit_report" EXIT

print_break_line()
{
	echo "";
	echo "===================================";
	echo "";
}

exit_report() {
	rv=$?					# return value

	if [ $rv -gt 1 ]; then
		echo "";
		echo "=================================";
		echo "==                             ==";
		echo "==    Error: build failed      ==";
		echo "==                             ==";
		echo "==          (︶︹︶)           ==";
		echo "==                             ==";
		echo "=================================";
	fi

	if [ $rv -eq 0 ]; then
		echo "";
		echo "=================================";
		echo "==                             ==";
		echo "==      Done Sucessfully       ==";
		echo "==                             ==";
		echo "==       *\_(^o^)_/*           ==";
		echo "==                             ==";
		echo "=================================";
	fi

	exit $rv
}

clean() {
	echo "Clean:";

	make clean;

	echo "Clean [ done ]";
	print_break_line
}

build() {
	echo "Build:";
	make;

	echo "Build [ done ]";
	print_break_line
}

merge() {
	echo "Merge:";

	mergehex -m ./output/nrf51422_xxac.hex ../nRF5_SDK/components/softdevice/s130/hex/s130_nrf51_2.0.1_softdevice.hex -o ./output/nrf51422_xxac_with_softdevice.hex

	echo "Merge: [done]";
	print_break_line
}

flash() {
	echo "Flash:";

	# erase device
	nrfjprog --family nRF51 -e;

	# flash binary
	nrfjprog --family nRF51 --program output/nrf51422_xxac_with_softdevice.hex

	# restart device
	nrfjprog --family nRF51 -r;

	echo "Flash: [ done ]"
	print_break_line
}


#####################################
#									#
# entry point						#
#									#
#####################################
clean;
build;
merge;
flash;



