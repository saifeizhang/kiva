/* Copyright (c) 2012-2016 Big Ladder Software. All rights reserved.
* See the LICENSE file for additional terms and conditions. */

#ifndef Domain_HPP
#define Domain_HPP

#include "Foundation.hpp"
#include "Mesher.hpp"
#include "Functions.hpp"

#include <fstream>
#include <numeric>

#include <boost/assign/list_of.hpp>

namespace Kiva {

class Cell
{
public:

  // inherent properties
  double density;
  double specificHeat;
  double conductivity;

  double volume;
  double area;
  double heatGain;

  // derived properties
  double cxp_c;
  double cxm_c;
  double cxp;
  double cxm;
  double cyp;
  double cym;
  double czp;
  double czm;

  // organizational properties
  enum CellType
  {
    EXTERIOR_AIR,  // 0
    INTERIOR_AIR,  // 1
    NORMAL,  // 2
    BOUNDARY,  // 3
    ZERO_THICKNESS  // 4
  };
  CellType cellType;

  //size_t blockNumber;
  Block block;

  //size_t surfaceNumber;
  Surface surface;
};

class Domain
{
public:

    // mesh
    Mesher meshX;
    Mesher meshY;
    Mesher meshZ;
    std::size_t nX;
    std::size_t nY;
    std::size_t nZ;

    std::vector<std::vector<std::vector<Cell>>> cell;

public:

    Domain();
    Domain(Foundation &foundation);
    void setDomain(Foundation &foundation);
    double getDXP(std::size_t i);
    double getDXM(std::size_t i);
    double getDYP(std::size_t j);
    double getDYM(std::size_t j);
    double getDZP(std::size_t k);
    double getDZM(std::size_t k);
    double getKXP(std::size_t i,std::size_t j,std::size_t k);
    double getKXM(std::size_t i,std::size_t j,std::size_t k);
    double getKYP(std::size_t i,std::size_t j,std::size_t k);
    double getKYM(std::size_t i,std::size_t j,std::size_t k);
    double getKZP(std::size_t i,std::size_t j,std::size_t k);
    double getKZM(std::size_t i,std::size_t j,std::size_t k);
    int getNumZeroDims(std::size_t i,std::size_t j,std::size_t k);
    void set2DZeroThicknessCellProperties(std::size_t i,std::size_t j,std::size_t k);
    void set3DZeroThicknessCellProperties(std::size_t i,std::size_t j,std::size_t k);
    void setZeroThicknessCellProperties(std::size_t i, std::size_t j, std::size_t k,
        std::vector<boost::tuple<std::size_t,std::size_t,std::size_t> > pointSet);
    void printCellTypes();

};

}

#endif
