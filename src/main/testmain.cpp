/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testmain.cpp
 * Author: ali_nayeem
 *
 * Created on December 24, 2018, 8:37 PM
 */

#include <cstdlib>
#include <iostream>
#include <InferSpeciesTree.h>

using namespace std;

/*
 * 
 */
string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

int main(int argc, char** argv) {

//    cout << "Hello\n";
//    string ls = GetStdoutFromCommand("java -jar lib/ASTRAL/astral.5.6.3.jar -q data/dummy/mpest_species.tre   -i data/dummy/gene.tre -t 0 2> log");
//    cout << ls << endl;
    string data = "data/R1";
    InferSpeciesTree  * problem = new InferSpeciesTree(data, 3);
    SolutionSet * pop =  problem->createInitialPopulation(1000);
    pop->printVariablesToFile(data+"/VAR");
//    map<string, void *> parameters;
//    parameters.clear();
//    double prob = 1.0; int numDes = 1;
//    parameters["probability"] = &prob;
//    parameters["numDescendientes"] = &numDes;
//    cout<<*(int*)parameters["numDescendientes"];
//    TreeCrossover crossover = TreeCrossover(parameters);
//    Solution ** parents = new Solution*[2];
//    int parent1, parent2;
//    //vector<Solution> children, parents;   
//    parents[0] = pop->get(4);
//    parents[1] = pop->get(5);
//    Solution * offSpring = (Solution *) (crossover.execute(parents));
    
    
    return 0;
}

