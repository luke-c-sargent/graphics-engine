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
  for(int i=0; i < 10000000000;++i){
    if(g.tkp->get_current_time()._nanos > 999999999){
      std::cout << "SHIT MAN ITS TOO MUCH\n";
    }
  }
}