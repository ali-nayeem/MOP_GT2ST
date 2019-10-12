//  PseudoRandom.h
//
//  Author:
//       Juan J. Durillo <durillo@lcc.uma.es>
//       Esteban López-Camacho <esteban@lcc.uma.es>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _PseudoRandom_h
#define _PseudoRandom_h

#include <iostream>
#include <RandomGenerator.h>
#include <Bpp/Numeric/Random/RandomTools.h>
#include <unordered_map> 
#include <thread>
#include <Bpp/Numeric/Random/Uniform01K.h>


using namespace bpp; 

/**
 * This file is aimed at defining the interface for the random generator. 
 * The idea is that all the random numbers will be generated using a single
 * random generator which will be accesible throug this interface.
 **/

class PseudoRandom {
//private:
public:
  static RandomGenerator * randomGenerator_;
  static RandomTools * bppRand_;
  static std::unordered_map <std::thread::id, RandomFactory*> RndFactoryMap;

  PseudoRandom();

public:
  static double randDouble();//    static int randInt();
  static int randInt(int minBound, int maxBound);
  //static int randInt(int maxBound);
  static double randDouble(double minBound, double maxBound);
  static RandomFactory* getRndFactory();
};


#endif
