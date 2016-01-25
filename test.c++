#include <stdio.h>
#include <time.h>
#include <string>
#include <unistd.h>

#include "TimeKeeper.h"
#include "LinuxTime.h"
#include "Game.h"

main(){
  Game g;
  g.tkp->print_time();
}