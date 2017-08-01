#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include "trajectory_iterator.h"
#include "math_vector.h"



int main(int argc, char**argv){
    /*
        Script to analyze a two nucleosome configuration
    */
    if (argc != 3){
        std::cout<<"Usage: "<<argv[0]<<" <dump file> <timeseries output file>"<<std::endl;
        exit(1);
    }

    // Load dump
    long long ntimestep,t;
    int natoms;
    int nuclA, nuclB;
    std::ofstream ofile;

    std::string dumpfilename;
    std::string outfilename;
    dumpfilename = argv[1];
    outfilename = argv[2];

    ofile.open(outfilename.c_str()); 

    //Set up all vectors needed for the trajectory parser class
    TrajectoryIterator parser;
    parser.load_dump(dumpfilename.c_str());

    std::vector<int> atom_types;
    std::vector<float> box_dim;
    std::vector<std::vector<double>> atoms;
    std::vector<std::vector<double>> quats;
    std::vector<std::vector<double>> vects_f;
    std::vector<std::vector<double>> vects_v;
    std::vector<std::vector<double>> vects_u;
    natoms = parser.get_numAtoms();
    atom_types = parser.get_type(); 
    ntimestep = parser.get_numFrames();

    // define vectors
    std::vector<double> r(3),rhat(3),fA(3),fB(3);
    double thetaA,thetaB,phi;
    double rnorm,fAdotfB, rdotfA, rdotfB;

    // define histograms
    std::vector<long long> count_in_angle_region(4,0);
    double thetaA_saddle=90., phi_saddle = 90.; // saddle points to define regions
    std::vector<long long> count_in_r_region(2,0);
    double rnorm_saddle = 140; //only bin values into histogram less than this distance

    bool firstframe = true;
    for(size_t i=0; i<ntimestep; i++) {
        if (firstframe){
          int nnucl = 0;
          for (size_t j=0; j<natoms; j++){
            if (atom_types[j] == 1){
              if (nnucl == 0) nuclA = j;
              else if (nnucl == 1) nuclB = j;
              else{
                std::cout << "Warning! More than two nucleosomes in dump file! Are you sure you're doing what you expect?" << std::endl;
              }
              nnucl ++;
            }
          }
          //std::cout << "Nucl ID are: " << nuclA << " " << nuclB << std::endl;

        }
        //The actual functions from the parser
        t = parser.get_current_timestep();
        atoms = parser.get_coord();
        quats = parser.get_quat();
        vects_f = parser.get_vect(quats,'f');
        
        fA = vects_f[nuclA];
        fB = vects_f[nuclB];
        
        r[0] = atoms[nuclB][0] - atoms[nuclA][0];
        r[1] = atoms[nuclB][1] - atoms[nuclA][1];
        r[2] = atoms[nuclB][2] - atoms[nuclA][2];
        rnorm = sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
        rhat[0] = r[0]/rnorm;
        rhat[1] = r[1]/rnorm;
        rhat[2] = r[2]/rnorm;

        rdotfA = rhat[0]*fA[0] +  rhat[1]*fA[1] +  rhat[2]*fA[2];
        if (rdotfA > 1) rdotfA =1;
        if (rdotfA <-1) rdotfA =-1;
        thetaA= acos(rdotfA) * 180. / M_PI;

        rdotfB = rhat[0]*fB[0] +  rhat[1]*fB[1] +  rhat[2]*fB[2];
        if (rdotfB > 1) rdotfB =1;
        if (rdotfB <-1) rdotfB =-1;
        thetaB= acos(rdotfB) * 180. / M_PI;

        fAdotfB = fA[0]*fB[0] +  fA[1]*fB[1] +  fA[2]*fB[2];
        if (fAdotfB > 1) fAdotfB =1;
        if (fAdotfB <-1) fAdotfB =-1;
        phi = acos(fAdotfB) * 180. / M_PI;
        

        //threshold rnorm
        if (rnorm < rnorm_saddle){ //bound r region
          ofile << t << " " <<rnorm << " " << phi << " " << thetaA << " " << thetaB << std::endl;
          //ofile << t << " " <<rnorm << " " << fAdotfB << " " << rdotfA << " " << rdotfB << std::endl;
          count_in_r_region[0]++;

          //now determine region
          if ((phi <  phi_saddle) && (thetaA >= thetaA_saddle)) count_in_angle_region[0]++;
          if ((phi <  phi_saddle) && (thetaA <  thetaA_saddle)) count_in_angle_region[1]++;
          if ((phi >= phi_saddle) && (thetaA >= thetaA_saddle)) count_in_angle_region[2]++;
          if ((phi >= phi_saddle) && (thetaA <  thetaA_saddle)) count_in_angle_region[3]++;
        }
        else{ //unbound r region
          count_in_r_region[1]++;
        }

        parser.next_frame();
        if (firstframe) firstframe = false;
    }  
    ofile.close();

    long long sum=0; 
    for (size_t i=0;i<2;i++) sum += count_in_r_region[i];
    for (size_t i=0;i<2;i++){ 
      double prob = (double)count_in_r_region[i]/sum;
      std::cout << prob << " ";
    }
    sum=0; 
    std::vector<double> prob_in_region(4);
    for (size_t i=0;i<4;i++) sum += count_in_angle_region[i];
    for (size_t i=0;i<4;i++){ 
      prob_in_region[i] = (double)count_in_angle_region[i]/sum;
    }
    std::cout << prob_in_region[0] << " " <<  prob_in_region[1] << " " <<  prob_in_region[2]+prob_in_region[3] <<std::endl;
    //std::cout << std::endl;

}
