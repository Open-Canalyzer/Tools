#include "slcan.h"

#include <stdio.h>
#include <stdint.h>


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

  //Setup the can mode

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
    case SLCAN_CMD_OPEN:
      break;
    case SLCAN_CMD_CLOSE:
      break;
    case SLCAN_CMD_TX11:
      break;
    case SLCAN_CMD_AUTOPOLL:
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
