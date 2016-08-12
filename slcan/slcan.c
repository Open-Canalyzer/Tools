#include "slcan.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool AutoPoll = false;
bool CanOpened = false;


void PrintCommand(const char* Command, int size)
{
  printf("Command(%d): ", size);
  for(int i = 0; i<size; i++)
  {
    printf("%c", Command[i]);
  }
  printf("\n");
}

int GetCommandSize(const char* in)
{
  int size = 0;

  while(*in != '\0')
  {
    size++;
    if(*in == '\r')
    {
      break;
    }
    in++;
  }

  return size;
}

uint32_t GetNumberOfCommands(const char* in)
{
  uint32_t i = 0;
  for (i=0; in[i]; in[i]=='\r' ? i++ : *in++);
  return i;
}

char CanSetup(char Mode)
{
  char ret = SLCAN_OK;
  
  if(Mode < '0' || Mode > '9')
    ret = SLCAN_ERR;

  //Check if the can bus is actually closed;
  if(CanOpened)
    ret = SLCAN_ERR;

  //Setup the can mode
  if(ret != SLCAN_ERR)
  {
    //Setup the CAN mode
    //It probably needs to be saved until OpenCanBus is called
  }

  return ret;
}

char OpenCanBus(char mode)
{
  char ret = SLCAN_OK;

  switch (mode)
  {
    case SLCAN_CMD_OPEN:    //Implement
    case SLCAN_CMD_LISTEN:
        CanOpened = true;
      break;
    default:
      ret = SLCAN_ERR;
  }

  return ret;
}

char CloseCanBus(void)
{
  char ret = SLCAN_OK;

  CanOpened = false;

  return ret;
}

//AutoPollMode is the preferred way of working
char AutoPollMode(char mode)
{
  char ret = SLCAN_OK;

  if(CanOpened)
    ret = SLCAN_ERR;

  if(ret != SLCAN_ERR)
  {
    if(mode == '0')
      AutoPoll = false;
    else if(mode == '1')
      AutoPoll = true;
    else
      ret = SLCAN_ERR;
  }

  return ret;
}

char ParseCommand(const char* in, int commandsize)
{
  commandsize -= 1; //dont care about the CR
  char ret = SLCAN_ERR;

  switch(in[0])
  {
    case SLCAN_CMD_SETUP:
      ret = CanSetup(in[1]);
      break;
    case SLCAN_CMD_SETUP_BTR:
      ret = SLCAN_ERR_NOT_IMPLEMENTED;
      break;
    case SLCAN_CMD_OPEN:
    case SLCAN_CMD_LISTEN:
      ret = OpenCanBus(in[0]);
      break;
    case SLCAN_CMD_CLOSE:
      ret = CloseCanBus();
      break;
    case SLCAN_CMD_TX11:
      break;
    case SLCAN_CMD_TX29:
      break;
    case SLCAN_CMD_RTR11:
      break;
    case SLCAN_CMD_RTR29:
      break;
    case SLCAN_CMD_POLL_ONE:
      break;
    case SLCAN_CMD_POLL_MANY:
      break;
    case SLCAN_CMD_FLAGS:
      break;
    case SLCAN_CMD_FILTER:
      ret = SLCAN_ERR_NOT_IMPLEMENTED;
      break;
    case SLCAN_CMD_ACC_CODE:
      ret = SLCAN_ERR_NOT_IMPLEMENTED;
      break;
    case SLCAN_CMD_ACC_MASK:
      ret = SLCAN_ERR_NOT_IMPLEMENTED;
      break;
    case SLCAN_CMD_UART:
      ret = SLCAN_ERR_NOT_IMPLEMENTED;
      break;
    case SLCAN_CMD_VERSION1:
    case SLCAN_CMD_VERSION2:
      break;
    case SLCAN_CMD_SERIAL:
      break;
    case SLCAN_CMD_TIMESTAMP:
      break;
    case SLCAN_CMD_AUTOSTART:
      ret = SLCAN_ERR_NOT_IMPLEMENTED;
      break;
    case SLCAN_CMD_AUTOPOLL:
      ret = AutoPollMode(in[1]);
      break;
    default:
      ret = SLCAN_ERR_UNKNOWN_CMD;
      break;
  }

  return ret;
}

int RunStateMachine(const char* in, char* out, int* parsed)
{
  *parsed = 0;

  uint32_t NumberOfCommands = GetNumberOfCommands(in);

  printf("NumberOfCommands: %d\n", NumberOfCommands);

  for(uint32_t i = 0; i<NumberOfCommands; i++)
  {
    int CommandSize = GetCommandSize(in);
    PrintCommand(in, CommandSize);
    ParseCommand(in, CommandSize);
    in += CommandSize;
    *parsed += CommandSize;
  }

  return 0;
}
