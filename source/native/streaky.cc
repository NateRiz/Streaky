//  This file is part of Streaky
//  Copyright (C) Nathan Rizik, 2019.
//  Released under GNU General Public License v3; see LICENSE

#include <iostream>
#include "../StreakyWorld.h"
#include "../StreakyWorld.cc"

// This is the main function for the NATIVE version of Streaky.
int main(int argc, char* argv[])
{
  StreakyWorld streakyWorld;
 
  streakyWorld.Start();
  for (unsigned int i = 0; i < 64; ++i){
    std::cout << "_______Time"<<i << "________\n";
    streakyWorld.Tick();
  }
  
}
