//  This file is part of Streaky
//  Copyright (C) Nathan Rizik, 2019.
//  Released under GNU General Public License v3; see LICENSE

#include <iostream>
#include "../StreakyWorld.cc"
#include "hardware/EventDrivenGP.h"

/**
 *
 * 10101010101010101010
 * 11110000111100001111
 *
 *
 *
 *
 */

// This is the main function for the NATIVE version of Streaky.
int main(int argc, char* argv[])
{
  const unsigned int SAMPLES = 1000;
  StreakyWorld streakyWorld;
  streakyWorld.CreatePopulation(SAMPLES);
  streakyWorld.Start();
  std::cout << "DONE." <<std::endl;
}

