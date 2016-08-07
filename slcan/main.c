#include <stdio.h>

#include "slcan.h"

int main(int argc, char** argv)
{
  printf("Hello World!\n");

  const char* Command = "S8\rX1\rO\rt123255AA\rC";
  char RetCommand[500];
  int parsed = 0;
  RunStateMachine(Command, RetCommand, &parsed);

  return 0;
}
