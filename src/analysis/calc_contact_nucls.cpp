#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include "trajectory_iterator.h"

int main(int argc, char**argv){
    if (argc != 5){
        std::cout<<"Usage: "<<argv[0]<<" <dump file> <output file> <cutoff dist in angstroms> <fraction of data>"<<std::endl;
        exit(1);
    }

    //Declare variables here
    long long ntimestep,t,ncalcstep; //Number of snapshots, iterator for current timestep
    int natoms; //Number of atoms
    std::string dumpfilename; //Name of the dumpfile (usually traj.dump)
    std::string outfilename; //Name of output file
    int rcut; //Cutoff radius for calculating contact map
    float dfrac; //data fraction

    //Parse all inputs
    dumpfilename = argv[1];
    outfilename = argv[2];
    rcut = std::stoi(argv[3]); //Cutoff for the contact maps
    dfrac = std::stof(argv[4]); //Fraction of the data to use

    if (dfrac > 1.0 || dfrac < 0.0) {
        std::cout<<"Error: Fraction is greater than possible. Exiting...."<<std::endl;
        exit(1);
    }

    //Set up all vectors needed for the trajectory parser class
    TrajectoryIterator parser;
    parser.load_dump(dumpfilename.c_str());

    //Get the number of atoms, box dimensions, and number of frames
    std::vector<float> box_dim;
    natoms = parser.get_numAtoms();
    ntimestep = parser.get_numFrames();

    //Initialize the contact and nucl ids here
    int nnucl; 
    std::vector<int> nucl_ids;
    std::vector<std::vector<double>> contacts; 
    bool firstframe = true;
    double sum = 0;

    //Loop through the dump file using the parser
    for(size_t i=0; i<ncalcstep; i++) {
        parser.next_frame();
        if (firstframe){
            nnucl = 0;
            std::vector<int> types = parser.get_types();
            for (size_t j=0;j<natoms;j++){
              if (types[j] == 1){ //is nucleosome
                nucl_ids.push_back(j+1);
                nnucl++;
              }
            }
            //resize the 2d contact map data
            contacts.resize(nucl_ids.size());
            for (size_t j=0;j<nucl_ids.size();j++) {
                contacts[j].resize(nucl_ids.size());
            }
        }
        if (firstframe) firstframe = false;
    }

    //Loop through the dump file using the parser
    for(size_t i=0; i<ncalcstep; i++) {
        parser.next_frame();
        if (firstframe){
            nnucl = 0;
            std::vector<int> types = parser.get_types();
            for (size_t j=0;j<natoms;j++){
              if (types[j] == 1){ //is nucleosome
                nucl_ids.push_back(j+1);
                nnucl++;
              }
            }
            //resize the 2d contact map data
            contacts.resize(nucl_ids.size());
            for (size_t j=0;j<nucl_ids.size();j++) {
                contacts[j].resize(nucl_ids.size());
            }
        }
        if (firstframe) firstframe = false;
    }

    //Second loop for the frames that are included in the contact map
    for(size_t i=ncalcstep; i<ntimestep; i++) {
       
        parser.next_frame();
        t = parser.get_current_timestep();

        if (firstframe){
            nnucl = 0;
            std::vector<int> types = parser.get_types();
            for (size_t j=0;j<natoms;j++){
              if (types[j] == 1){ //is nucleosome
                nucl_ids.push_back(j+1);
                nnucl++;
              }
            }
            //resize the 2d contact map data
            contacts.resize(nucl_ids.size());
            for (size_t j=0;j<nucl_ids.size();j++) {
                contacts[j].resize(nucl_ids.size());
            }
        }
  
        //compute contact map
        double dist = 0;
        int nuclj, nuclk;
        for(size_t j=0;j<nnucl;j++){
          for(size_t k=0;k<nnucl;k++){
            nuclj = nucl_ids[j];
            nuclk = nucl_ids[k];
            dist = parser.get_dist(nuclj,nuclk);
            if(dist < rcut) {contacts[j][k] += 1.0;}
          }
        }
        
        if (firstframe) firstframe = false;
        sum++;
    }  

    //Declare initial file data here
    std::ofstream ofile;
    ofile.open(outfilename.c_str());
    ofile << "#Contact map data stored here" << std::endl;

    //Average the contacts over number of snapshots and add to file
    ofile << "#" << t << "\t" << sum << std::endl;
    for(size_t i=0;i<nucl_ids.size();i++) {
        for(size_t j=0;j<nucl_ids.size();j++) {
            if (contacts[i][j] == 0) {sum = 0;}
            else {sum = log(contacts[i][j]);} //The log of the number of contacts is stored
            ofile << sum << "\t";
        }
        ofile << std::endl;
    } 
    ofile.close(); //Close the file
}
