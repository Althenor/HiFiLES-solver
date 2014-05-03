/*!
 * \file cubature_1d.cpp
 * \brief _____________________________
 * \author - Original code: SD++ developed by Patrice Castonguay, Antony Jameson,
 *                          Peter Vincent, David Williams (alphabetical by surname).
 *         - Current development: Aerospace Computing Laboratory (ACL) directed
 *                                by Prof. Jameson. (Aero/Astro Dept. Stanford University).
 * \version 1.0.0
 *
 * HiFiLES (High Fidelity Large Eddy Simulation).
 * Copyright (C) 2013 Aerospace Computing Laboratory.
 */

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

#include "../include/global.h"
#include "../include/cubature_1d.h"

using namespace std;

// #### constructors ####

// default constructor

cubature_1d::cubature_1d()
{	
  order=0;
  n_pts=0;
  locs.setup(0);
  weights.setup(0);
}

// constructor 1

cubature_1d::cubature_1d(int in_order) // set by number of points
{	
  ifstream datfile;
  char buf[BUFSIZ]={""};
  char section_TXT[100], param_TXT[100];
  char* f;
  string filename, param_name, param, ord;
  istringstream strbuf;
  int order_file;

  order=in_order;
  n_pts = (order+1)/2;
  locs.setup(n_pts);
  weights.setup(n_pts);
  
  if(order < 13) {

    // get env var specifying location of data directory?
    //const char* HIFILES_DATADIR = getenv("HIFILES_DATA");
    //filename = HIFILES_DATADIR;
    //filename += "/cubature_1d_new.dat";
    //cout << filename << endl;

    if (HIFILES_DIR == NULL)
      FatalError("environment variable HIFILES_HOME is undefined");

    filename = HIFILES_DIR;
    filename += "/data/cubature_1d.dat";
    f = (char*)filename.c_str();
    datfile.open(f, ifstream::in);
    if (!datfile) {
        cout << "Cubature file name: " << filename << endl;
        FatalError("Unable to open cubature file");
    }
    
    // read data from file to arrays
    while(datfile.getline(buf,BUFSIZ))
    {
      sscanf(buf,"%s",&section_TXT);
      param_name.assign(section_TXT,0,99);
      
      if(!param_name.compare(0,5,"order"))
      {
        // get no. of pts
        ord = param_name.substr(6);
        stringstream str(ord);
        str >> order_file;
        
        // if pts matches order, read locs and weights
        if (order_file == order) {
          
          // skip next line
          datfile.getline(buf,BUFSIZ);
          
          for(int i=0;i<n_pts;++i) {
            datfile.getline(buf,BUFSIZ);
            sscanf(buf,"%s",&param_TXT);
            param.assign(param_TXT,0,99);
            strbuf.str(param);
            locs(i) = atof(param.c_str());
          }

          // skip next line
          datfile.getline(buf,BUFSIZ);

          for(int i=0;i<n_pts;++i) {
            datfile.getline(buf,BUFSIZ);
            sscanf(buf,"%s",&param_TXT);
            param.assign(param_TXT,0,99);
            strbuf.str(param);
            weights(i) = atof(param.c_str());
          }
          break;
        }
      }
    }
  }
}

// copy constructor

cubature_1d::cubature_1d(const cubature_1d& in_cubature_1d)
{
  order=in_cubature_1d.order;
  n_pts=in_cubature_1d.n_pts;
  locs=in_cubature_1d.locs;
  weights=in_cubature_1d.weights;
}

// assignment

cubature_1d& cubature_1d::operator=(const cubature_1d& in_cubature_1d)
{
  // check for self asignment
  if(this == &in_cubature_1d)
    {
      return (*this);
    }
  else
    {
      order=in_cubature_1d.order;
      n_pts=in_cubature_1d.n_pts;
      locs=in_cubature_1d.locs;
      weights=in_cubature_1d.weights;
    }
}

// destructor

cubature_1d::~cubature_1d()
{

}

// #### methods ####

// method to get number of cubature_1d points

int cubature_1d::get_n_pts(void)
{
  return n_pts;
}

// method to get r location of cubature_1d point

double cubature_1d::get_r(int in_pos)
{
  return locs(in_pos);
}

// method to get weight location of cubature_1d point

double cubature_1d::get_weight(int in_pos)
{
  return weights(in_pos);
}
