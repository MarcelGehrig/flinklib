#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <flinklib.h>

#define DEFAULT_DEV "/dev/flink0"

int main(int argc, char* argv[]) {
	flink_t* dev;
	char* dev_name = DEFAULT_DEV;
	uint8_t subdevice = 0;
	uint32_t channel = 0;
	uint32_t value = 0;
	uint32_t repeat = 1;
	char c;
	int error = 0;
	
	/* Compute command line arguments */
	while((c = getopt(argc, argv, "d:s:c:n:")) != -1) {
		switch(c) {
			case 'd': // device
				dev_name = optarg;
				break;
			case 's':
				subdevice = atoi(optarg);
				break;
			case 'c':
				channel = atoi(optarg);
				break;
			case 'n':
				repeat = atoi(optarg);
				break;
			case '?':
				if(optopt == 'd' || optopt == 'c' || optopt == 'n') fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if(isprint(optopt)) fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				return -1;
			default:
				abort();
		}
	}
	
	printf("Opening device %s...\n", dev_name);
	dev = flink_open(dev_name);
	if(dev == NULL) {
		printf("Failed to open device!\n");
		return -1;
	}
	
	printf("Reseting counter subdevice...\n");
	error = flink_subdevice_reset(dev, subdevice);
	if(error != 0) {
		printf("Resetting failed!\n");
		return -1;
	}
	
	printf("Reading value from counter %u at subdevice %u...\n", channel, subdevice);
	while(repeat-- > 0) {
		error = flink_counter_get_count(dev, subdevice, channel, &value);
		if(error != 0) {
			printf("Reading failed!\n");
			return -1;
		}
		else {
			printf("%u (0x%x)\n", value, value);
		}
		sleep(1);
	}
	
	printf("Closing device %s...\n", dev_name);
	flink_close(dev);
	
    return EXIT_SUCCESS;
}