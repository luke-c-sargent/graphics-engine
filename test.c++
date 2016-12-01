#include <stdio.h>
#include <time.h>
#include <string>
#include <unistd.h>

#include "TimeKeeper.h"
#include "LinuxTime.h"
#include "Game.h"
#include "Renderer/Vulkan.h"

#define VK_USE_PLATFORM_XCB_KHR

int main(){
  //Game g;
  //g.tkp->print_time();
  //g.tkp->print_time();
  //g.tkp->print_time();

  Vulkan v = Vulkan();

  return 0;
}