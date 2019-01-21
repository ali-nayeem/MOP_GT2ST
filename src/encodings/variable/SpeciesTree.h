/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SpeciesTree.h
 * Author: ali_nayeem
 *
 * Created on December 28, 2018, 9:34 PM
 */

#ifndef SPECIESTREE_H
#define SPECIESTREE_H

#include<PhyloTree.h>

//using namespace std;
class SpeciesTree : public PhyloTree {
public:
    SpeciesTree(string newickFile);
    //SpeciesTree(const SpeciesTree& orig);
    string toString() ;
    ~SpeciesTree();
private:

};

#endif /* SPECIESTREE_H */

