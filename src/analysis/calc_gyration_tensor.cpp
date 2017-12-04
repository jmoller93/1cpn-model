#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include "trajectory_iterator.h"



int main(int argc, char**argv){

    if (argc != 3){
        std::cout<<"Usage: "<<argv[0]<<" <dump file> <output file> "<<std::endl;
        exit(1);
    }

   
    // Load dump
    long long ntimestep,t;
    int natoms;

   std::string dumpfilename;
    std::string outfilename;
    dumpfilename = argv[1];
    outfilename = argv[2];

    std::ofstream ofile;
    ofile.open(outfilename.c_str());
    ofile << "# <timestep> <Sxx> <Syy> <Szz> <Rg>" << std::endl;

    //Set up all vectors needed for the trajectory parser class
    TrajectoryIterator parser;
    parser.load_dump(dumpfilename.c_str());

    std::vector<float> box_dim;
    //std::vector<std::vector<double>> vects_f;
    //std::vector<std::vector<double>> vects_v;
    //std::vector<std::vector<double>> vects_u;
    natoms = parser.get_numAtoms();
    ntimestep = parser.get_numFrames();

    double halfbox[3];

    int nnucl; 
    std::vector<double> gyr_tensor(3); //We are only going to evaluate the diagonals
    std::vector<double> com;
    std::vector<double> r(3); //distance vector com-rsite dist

    bool firstframe = true;
    //Loop through the dump file using the parser
    //for(size_t i=0; i<timestep; i++) {
    for(size_t i=0; i<ntimestep; i++) {
       
        parser.next_frame();
        box_dim = parser.get_boxDim();

        //vects_f = parser.get_vect('f');
        //vects_v = parser.get_vect(quats,'v');
        //vects_u = parser.get_vect('u');
        t = parser.get_current_timestep();

        //compute center of mass of each timestep
        com = parser.get_com();
        double sumx=0,sumy=0,sumz=0;

        //apply pbc
        halfbox[0] = 0.5* (box_dim[1] - box_dim[0]);
        halfbox[1] = 0.5* (box_dim[3] - box_dim[2]);
        halfbox[2] = 0.5* (box_dim[5] - box_dim[4]);

        //evaluate the gyration tensor now
        for(size_t j=0;j<natoms-1;j++){
          for(size_t k =0; k<3; k++){
            r[k] = parser.coords_[j][k]-com[k];
            if (r[k] >  halfbox[k]) r[k] -= halfbox[k];
            if (r[k] <- halfbox[k]) r[k] += halfbox[k];
          }
          sumx += r[0]*r[0];
          sumy += r[1]*r[1];
          sumz += r[2]*r[2];
          //dr = sqrt(dx*dx+dy*dy+dz*dz);
          //sum += 1.0/dr;
        }

        double Sxx = sumx*1.0/natoms;
        double Syy = sumy*1.0/natoms;
        double Szz = sumz*1.0/natoms;
        double Rg2 = Sxx + Syy + Szz;
        ofile << t<< "\t" <<Sxx << "\t" << Syy << "\t" << Szz << "\t" << Rg2 << std::endl;
    
        if (firstframe) firstframe = false;
    }  

    ofile.close();
   
}
