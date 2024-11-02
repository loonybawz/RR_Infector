#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>


int main(int argc, char **argv) {
	int status, cts;
	libusb_device_handle *dev = NULL;

	status = libusb_init(NULL);
	if(status != 0) {
		perror("Error init libusb");
		return -status;
	}


	dev = libusb_open_device_with_vid_pid(NULL, 0x054c, 0x01cb);
	if(dev == NULL) {
		printf("Error! Could not find USB device!\n");
		libusb_exit(NULL);
		return -1;
	}


	unsigned char echobuff[4] = {0, 0, 0, 3};
	status = libusb_control_transfer(dev, 0x41, 0x7, 0, 0, echobuff, 4, 100);
	if(status != 4) goto exit;


	status = libusb_control_transfer(dev, 0xC1, 8, 0, 0, (unsigned char *) &echobuff, 4, 100);
	if(status != 4) goto exit;


	unsigned char gameid[64];
	status = libusb_control_transfer(dev, 0xC1, 0x1, 0, 0, (unsigned char *) &gameid, 64, 100);
	if(status <= 8) {
		goto exit;
	} else {
		printf("GameID: %s\n",gameid);
	}


	unsigned char cmd1[12] = {2, 0, 0, 0, 2, 0, 0, 0, 0, 16, 0, 0};
	status = libusb_control_transfer(dev, 0x41, 0x2, 0, 0, cmd1, 12, 100);
	if(status != 12) goto exit;


	status = libusb_control_transfer(dev, 0x41, 0x3, 0x0001, 0, NULL, 0, 100);
	if(status != 0) goto exit;


	unsigned char bufferi[8];
	status = libusb_bulk_transfer(dev, 0x83, bufferi, 8, NULL, 100);

	cts = 0;
	status = libusb_control_transfer(dev, 0xC1, 4, 0xff00, 0, (unsigned char *) &cts, 1, 100);
	if(status != 1) goto exit;
	printf("Current CTS value: 0x%x\n", cts);
	if(cts == 0) goto exit;


	const char expectedid[] = "UCUS-98668";
	if(strcmp(gameid, expectedid) != 0){
		printf("Error! Wrong Game!\n");
		goto exit;
	}


	char buffer1[8] = {0, 0, 0x81, 0, 3, 0, 0, 0};
	char buffer2[3] = "21\n";
	libusb_bulk_transfer(dev, 2, buffer1, 8, NULL, 100);
	libusb_bulk_transfer(dev, 2, buffer2, 3, NULL, 100);
	printf("Infected!\n");
exit:
	libusb_close(dev);
	libusb_exit(NULL);
	return 0;
}
