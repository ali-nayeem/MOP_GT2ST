/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SpeciesTree.cpp
 * Author: ali_nayeem
 * 
 * Created on December 28, 2018, 9:34 PM
 */

#include "SpeciesTree.h"

SpeciesTree::SpeciesTree(string newickFile) : PhyloTree(newickFile) {
}
//
//SpeciesTree::SpeciesTree(const SpeciesTree& orig) {
//}
//
string SpeciesTree::toString(){
  stringstream ss;
  ss << TreeTemplateTools::treeToParenthesis(*getTree());
//        << "PARAMETROS" << endl  
//        << "Transition / transversion ratio (Kappa): " << kappa << endl 
//        << "Gamma Shape (alpha): " << alpha << " beta: "  << beta << endl
//        << "Frecuencias  piA: " << piA << " piC: " << piC << " piG: " << piG << " piT: " << piT << endl 
//        << "GTR relative rates AC: " << AC << " AG: " << AG << " AT: " << AT << " CG: " << CG << " CT: " << CT << " GT: " << GT << endl; 
        return ss.str();
} // toString

SpeciesTree::~SpeciesTree() {
}

