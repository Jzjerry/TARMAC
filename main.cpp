//
//  Created by Yangdi Lyu on 12/31/18.
//  Copyright © 2018 Yangdi Lyu. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <string>
#include <random>
#include <unistd.h>
#include <sys/stat.h>
#include <chrono>
#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include "interpreter.h"
#include "visitor.h"

/*
 * read vector from file
 * each line represent a vector
 * vecs: returned vectors
 */
void readVectorsFromFile(std::string filename, std::vector<std::string> &vecs);

/*
 * generate random vectors
 * length: size of each vector
 * patterns: total number of vectors
 */
void genRandVectors(std::string filename, size_t length, unsigned int patterns);

/*
 * create folder
 * return -1 if failed
 */
int createfolder(std::string foldername);

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " benchmark numRand numTriggers numTrojans numVectors trigthresh tool\n";
        return 1;
    }
    std::string bench = argv[1];
    unsigned int numRand = std::stoi(argv[2]);
    std::string tool = argv[3];
    // const unsigned int Ndetect = 1000;
    
    std::string testFolder = "tests";
    std::string debugFolder = "Debug";
    // std::string trojanFolder = "Trojans_theta_" + threshold;
    std::string benchFolder = "benchmarks";

    if (createfolder(testFolder) == -1 ||
        // createfolder(trojanFolder) == -1 ||
        createfolder(debugFolder) == -1)
        return -1;
    
    srand(42);
    
    Interpreter a(benchFolder + "/" + bench + ".v");
    auto start = std::chrono::system_clock::now();
    std::shared_ptr<Graph> g = nullptr;
    try {
        g = a.createGraph();
    } catch (std::string errormsg)
    {
        std::cerr << errormsg << std::endl;
        return 1;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "create graph (topo sorting, SAT expression) time: " << elapsed_seconds.count() << "s\n";

    std::vector<std::string> randvecs;
    std::string randFileName = testFolder + "/rand" + std::to_string(numRand) + "_PI_" + bench + ".txt";
    genRandVectors(randFileName, g->inEdges.size(), numRand);
    std::cout << "reading vectors from " + randFileName + "\n";
    readVectorsFromFile(randFileName, randvecs);
    
    std::cout << g->reportGraph();
    

    std::string vecFileName = testFolder + "/" + tool + "_PI_" + bench + "_N1000_" + ".txt";
    Visitor vst(g, vecFileName, 1);
    int saf_total = vst.countSafRemaining();
    std::cout << "SAF Total: " << g->edgArr.size() * 2 << " (2 * EdgeNum)\n";
    std::cout << "SAF To Detect: " << saf_total << std::endl;

    // TARMAC ATPG 
    if( tool == "TARMAC"){
        std::string atpgFileName = testFolder + "/TARMAC" + "_PI_" + bench + ".txt";
        std::vector<std::string> atpgvecs;
        std::ofstream fvec(atpgFileName.c_str()); 
        start = std::chrono::system_clock::now();
        vst.TARMAC_ATPG_naive(atpgvecs, numRand);
        elapsed_seconds = std::chrono::system_clock::now() - start;
        std::cout << "ATPG Generation time: " << elapsed_seconds.count() << "s\n";
        readVectorsFromFile(atpgFileName, atpgvecs);
        start = std::chrono::system_clock::now();
        for(const auto& vec : atpgvecs){
            vst.simOneVector(vec);
            for(auto &e : vst.faultyEdges){
                if (!(e->detected[0] && e->detected[1])) // TODO: This is performance heavy!
                    vst.safPropagated(e);
            }
            fvec << vec << std::endl;
        }
        fvec.close();
        elapsed_seconds = std::chrono::system_clock::now() - start;
        std::cout << "ATPG simulation time: " << elapsed_seconds.count() << "s (" << elapsed_seconds.count()/atpgvecs.size() << "s/vec, " << atpgvecs.size() <<" vectors)\n";
    }
    else if( tool == "RANDOM"){
        start = std::chrono::system_clock::now();
        for(const auto& vec : randvecs){
            vst.simOneVector(vec);
            for(auto &e : vst.faultyEdges){
                if (!(e->detected[0] && e->detected[1])) // TODO: This is performance heavy!
                    vst.safPropagated(e);
            }
        }
        elapsed_seconds = std::chrono::system_clock::now() - start;
        std::cout << "Random simulation time: " << elapsed_seconds.count() << "s (" << elapsed_seconds.count()/randvecs.size() << "s/vec, " << randvecs.size() <<" vectors)\n";

    }
    int saf_remain = vst.countSafRemaining();
    std::cout << "SAF Undetected: " << saf_remain << std::endl;
    // vst.printUndetectedSaf();
    std::cout << "Coverage: " << (1-(double)saf_remain/saf_total)*100 << "%\n";

    return 0;
}

void readVectorsFromFile(std::string filename, std::vector<std::string> &vecs) {
    std::ifstream fvec(filename);
    if (!fvec.is_open()) {
        throw std::string("Vector file not present: " + std::string(filename) + ". Exiting ...\n");
    }
    std::string line;
    while (fvec.good()) {
        fvec >> line;
        if (!fvec.good())    break;
        vecs.push_back(line);
    }
    fvec.close();
}

// Generate random test vectors
void genRandVectors(std::string filename, size_t length, unsigned int patterns) {
    // If file already exists, skip
    if ( access(filename.c_str(), F_OK ) != -1 ) {
        std::cout << "Random patterns: " << filename << " already exist. Skip generating...\n";
        return;
    }
    std::ofstream fout(filename);
    int blockshift = 4;
    int twopower = 1 << blockshift;
    int maxRand = (1 << twopower) - 1;
    // use better pseudo-random generator than rand() % 2
    std::mt19937 e(std::random_device{}());
    std::uniform_int_distribution<int> uniform_dist(0, maxRand);
    // generate enought bits to p
    size_t numBlock = ((length - 1) >> blockshift) + 1;
    std::string p(numBlock << blockshift, '0');
    
    std::cout << "Generating " << patterns << " random patterns in file: " << filename << " ...\n";
    
    for (unsigned int i = 0; i < patterns; ++i) {
        for (size_t j = 0; j < numBlock; ++j) {
            int r = uniform_dist(e);
            // extract 8 bits from r
            for (int k = 0; k < twopower; ++k) {
                p[(j << blockshift) + k] = (r & 1) ? '1' : '0';
                r >>= 1;
            }
        }
        
        fout << p.substr(0, length) << std::endl;
    }
    fout.close();
}

// create folder
int createfolder(std::string foldername) {
    if ((mkdir(foldername.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) && (errno != EEXIST)) {
        std::cerr << "Error creating folder " << foldername << std::endl;
        return -1;
    }
    return 0;
}
