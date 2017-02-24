#include <iostream>
#include <libusb.h>
#include <unistd.h>

#include <pb_decode.h>
#include "proto/protocols.pb.h"

//inspired from https://github.com/tytouf/libusb-cdc-example/blob/master/cdc_example.c

// for OSX sudo kextunload -b com.apple.driver.usb.cdc.acm

namespace
{
  uint16_t VendorId = 0x0483;
  uint16_t ProductId = 0x5740;
  struct libusb_device_handle *devh = NULL;
  int ep_in_addr  = 0x81;
  int ep_out_addr = 0x82;
}

#define ACM_CTRL_DTR   0x01
#define ACM_CTRL_RTS   0x02

int read_chars(unsigned char * data, int size)
{
  /* To receive characters from the device initiate a bulk_transfer to the
   * Endpoint with address ep_in_addr.
   */
  int actual_length;
  int rc = libusb_bulk_transfer(devh, ep_in_addr, data, size, &actual_length,
                                1000);
  if (rc == LIBUSB_ERROR_TIMEOUT) {
      printf("timeout (%d)\n", actual_length);
      return -1;
  } else if (rc < 0) {
      fprintf(stderr, "Error while waiting for char\n");
      return -1;
  }

  return actual_length;
}


int main(int argc, char** argv)
{
  printf("Hello World!\n");

  int rc;
  unsigned char encoding[] = { 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08 };

  rc = libusb_init(NULL);
  if (rc < 0) 
  {
    fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(rc));
    exit(1);
  }

  libusb_set_debug(NULL, 3);

  devh = libusb_open_device_with_vid_pid(NULL, VendorId, ProductId);
  if (!devh)
  {
    fprintf(stderr, "Error finding USB device\n");
    goto out;
  }
  /* As we are dealing with a CDC-ACM device, it's highly probable that
   * Linux already attached the cdc-acm driver to this device.
   * We need to detach the drivers from all the USB interfaces. The CDC-ACM
   * Class defines two interfaces: the Control interface and the
   * Data interface.
   */
  for (int if_num = 0; if_num < 2; if_num++)
  {
    if (libusb_kernel_driver_active(devh, if_num)) 
    {
      libusb_detach_kernel_driver(devh, if_num);
    }
    rc = libusb_claim_interface(devh, if_num);
    if (rc < 0)
    {
      fprintf(stderr, "Error claiming interface: %s\n", libusb_error_name(rc));
      goto out;
    }
  }

  /* Start configuring the device:
   * - set line state
   */
  rc = libusb_control_transfer(devh, 0x21, 0x22, ACM_CTRL_DTR | ACM_CTRL_RTS,0, NULL, 0, 0);
  if (rc < 0)
  {
    fprintf(stderr, "Error during control transfer: %s\n", libusb_error_name(rc));
  }

  /* - set line encoding: here 9600 8N1
   * 9600 = 0x2580 ~> 0x80, 0x25 in little endian
   */
  
  rc = libusb_control_transfer(devh, 0x21, 0x20, 0, 0, encoding, sizeof(encoding), 0);
  if (rc < 0)
  {
    fprintf(stderr, "Error during control transfer: %s\n",
    libusb_error_name(rc));
  }

  unsigned char buf[65];
  int len;
  
  for(int i = 0; i<100; i++)
  {
    CanMessage RecCanMessage = CanMessage_init_default;
    len = read_chars(buf, 64);
    buf[len] = 0;
    pb_istream_t stream = pb_istream_from_buffer(buf, len);
    //fprintf(stdout, "Received: \"%s\"\n", buf);

    if (pb_decode(&stream, CanMessage_fields, &RecCanMessage))
    {
      printf("DLC: %u RTR: %u IDE: %u ID: 0x%x Data1: 0x%X Data2 0x%X\n", 
        RecCanMessage.DLC, RecCanMessage.RTR, RecCanMessage.IDE,
        RecCanMessage.ID, RecCanMessage.Data1, RecCanMessage.Data2);
    }
    //sleep(1);
  }

  libusb_release_interface(devh, 0);

out:
  if (devh)
    libusb_close(devh);
  libusb_exit(NULL);
  return rc;
}
