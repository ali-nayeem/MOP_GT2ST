/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InferSpeciesTree.cpp
 * Author: ali_nayeem
 *
 * Created on December 28, 2018, 10:09 AM
 */

#include <InferSpeciesTree.h>
#include <unordered_set>
#include <iterator>

int InferSpeciesTree::instanceCount = 0;
InferSpeciesTree::InferSpeciesTree(string & _datapath, vector <int> & _selectedObjectives) {

    this->datapath =  "data/" +  _datapath + "/";
    numberOfVariables_ = 1;
    selectedObjectives = _selectedObjectives;
    numberOfObjectives_ = selectedObjectives.size();
    objMin = new double[numberOfObjectives_];
    objMax = new double[numberOfObjectives_];
    for(int i=0; i<numberOfObjectives_; i++)
    {
        objMin[i] = 1.0e+30; //std::numeric_limits<double>::max();
        objMax[i] = -1.0e+30; //std::numeric_limits<double>::min();
        //cout << "Initial value of min, max: "<<objMin[i]<<", "<<objMax[i]<<endl;
    }
    numberOfConstraints_ = 0;
    problemName_ = "Infer Species Tree: " + _datapath;
    solutionType_ = new PhyloTreeSolutionType(this);
    treeFiles.push_back(datapath + speciesTreeFileName + "_astral");
    treeFiles.push_back(datapath + speciesTreeFileName + "_mpest");
    //treeFiles.push_back(datapath + speciesTreeFileName + "_phylonet");
    treeFiles.push_back(datapath + speciesTreeFileName + "_stelar");
    //newick = new Newick;
    timestamp_ = time(0);//*100 + instanceCount++;//time(0);
    threadId_ = -1;
    //readPrecomputedSpeciesTree();
    trueTree = getSolutionSetFromVarFile(datapath + trueTreeFileName)->get(0);
    //cout << trueTree->toString();
    this->numberOfTaxa_ = ((PhyloTree*)(trueTree->getDecisionVariables()[0]))->getNumberOfLeaves();  
    string cmdOut = GetStdoutFromCommand("uname");
    
    string mac = "Darwin";
    if(cmdOut.find(mac) != string::npos)
    {
        os = "mac";
    }
    else
    {
        os = "linux";
    }

    //GenerateInitialTrees();

    //if(printtrace) OpenScores();


} // InferSpeciesTree


vector< PhyloTree* > InferSpeciesTree::readPrecomputedSpeciesTree() {
    for (int i = 0; i < treeFiles.size(); i++) {
        
        if (fileExistsTest(treeFiles.at(i))) {
            #ifdef MAN_DEBUG
                cout << "Reading precomputed tree: "<< treeFiles[i] << endl;
            #endif
            precomputedTrees.push_back(new PhyloTree(treeFiles[i]));
        }        
    }
    if(precomputedTrees.size() < 2){
        throw Exception("More than one PreComputedSpeciesTree is needed");
    }
    return precomputedTrees;
}
int InferSpeciesTree::getNumberOfTaxa()
{
    return numberOfTaxa_;
}

void InferSpeciesTree::setBranchLength(SolutionSet *pop, double len)
{
    for(int i=0; i<pop->size(); i++)
    {
        Solution * sol = pop->get(i);
        TreeTemplate<Node> * tree = ((PhyloTree *) sol->getDecisionVariables()[0])->getTree();
        TreeTemplateTools::setBranchLengths(*tree->getRootNode(), len);
    }
}

SolutionSet * InferSpeciesTree::getSolutionSetFromVarFile(string varFileName)
{
    ifstream varFile(varFileName);
    // new lines will be skipped unless we stop it from happening:    
    varFile.unsetf(std::ios_base::skipws);

    // count the newlines with an algorithm specialized for counting:
    unsigned lineCount = std::count(
        std::istream_iterator<char>(varFile),
        std::istream_iterator<char>(), 
        '\n');
    varFile.clear();
    varFile.seekg(0, ios::beg);

    SolutionSet * pop = new SolutionSet(lineCount);
    string aTree;
    while(std::getline(varFile, aTree))
    {
        TreeTemplate<Node> * tree = TreeTemplateTools::parenthesisToTree(aTree); //, true, TreeTools::BOOTSTRAP, false, false
        TreeTemplateTools::deleteBranchLengths(*tree->getRootNode());
        Variable **variables = new Variable*[this->getNumberOfVariables()];
        variables[0] = new PhyloTree();
        ((PhyloTree *) variables[0])->setTree(tree);
        Solution * newSolution = new Solution(this, variables);
        pop->add(newSolution);
    }
    //cout<<pop->size();
    varFile.close();
    return pop;
}



boolean InferSpeciesTree::PLLisTreeValidate(TreeTemplate<Node> * tree){
  pllNewickTree * newick;
  boolean res=true;
  string treenewick = TreeTemplateTools::treeToParenthesis(*tree) ;
//  pll_rtree_t * rtree;
//  if(!(rtree=pll_rtree_parse_newick_string(treenewick.c_str())))
//  {
//      //cout<<"Failed to parse as rooted tree. ";
//      return false;
//  }
//  else
//  {
//      //cout<<"Tip count: "<<rtree->tip_count<<endl;
//      //pll_rtree_destroy(rtree, NULL)
//      return true;
//  }
//  pll_utree_t * utree;
//  if(!(utree=pll_utree_parse_newick_string(treenewick.c_str())))
//  {
//      cout<<"Failed to parse as unrooted tree."<<endl;
//  }
  newick = pllNewickParseString	(treenewick.c_str());
  if (!newick){
      res = false;
  }else {
      //if (!pllValidateNewick (newick))  res=false;  //if (!pllValidateNewick (newick))  !pllNewickUnroot(newick)
      return true;
  }
  //pllNewickParseDestroy (&newick);
  return res;
}

boolean InferSpeciesTree::PLLisTreeValidate(Solution * solution)
{
    Variable **variables = solution->getDecisionVariables();
    PhyloTree * Pt = (PhyloTree*) variables[0];
    TreeTemplate<Node> * tree = Pt->getTree();
    return PLLisTreeValidate(tree);
}

int InferSpeciesTree::getNumberOfLeaves(Solution * solution)
{
    Variable **variables = solution->getDecisionVariables();
    PhyloTree * Pt = (PhyloTree*) variables[0];
    TreeTemplate<Node> * tree = Pt->getTree();
    return tree->getNumberOfLeaves();
}

bool InferSpeciesTree::isMultifurcating(Solution * solution)
{
    Variable **variables = solution->getDecisionVariables();
    PhyloTree * Pt = (PhyloTree*) variables[0];
    TreeTemplate<Node> * tree = Pt->getTree();
    return tree->isMultifurcating();
} 
void InferSpeciesTree::evaluate(Solution *solution) {
    varFile_ = datapath + "tmp";
    if(!fileExistsTest(varFile_))
    {
        GetStdoutFromCommand("mkdir " + varFile_);
    }
    varFile_ += "/"+ to_string(timestamp_) + to_string(threadId_);
    std::ofstream out(varFile_);
    //solution-> ->printVariablesToFile(varFile_);
    out << solution->toString();
    out.close();
    for(int i=0; i<selectedObjectives.size(); i++)
    {
        int objId = selectedObjectives[i];
        string scoreList = (this->*(getScoreFunctions[objId]))(varFile_, 1);
        stringstream ss(scoreList);
        string to;
        //int solId=0;
        while(std::getline(ss,to,'\n')){
          double value = atof(to.c_str());
          solution->setObjective(i, value * objNegIfMax[objId]);
          //cout << pop->get(solId)->getObjective(objId) << endl;
          //solId++;
        }
    }
    updateReference(solution);
}
void InferSpeciesTree::setThreadId(int id)
{
    threadId_ = id;
}
string InferSpeciesTree::GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    //cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

string InferSpeciesTree::getAstralScoreList(string varFile, int popSize)
{
    //cout << "Getting Astral Score"<< endl;
    string ls = GetStdoutFromCommand("java -jar lib/ASTRAL/astral.5.6.3.jar -q " + varFile 
                                    + " -i " + datapath +  "gene.tre -o tmp/out 2> tmp/log");
    //cout << ls << endl;
    return ls;
}
string InferSpeciesTree::getPhylonetScoreList(string varFile, int popSize)
{
   //cout << "Getting Phylonet Score"<< endl;
   string ls = GetStdoutFromCommand("java -jar lib/phylonet/phylonet_v2_4.jar deep_coal_count " 
                                    + varFile + " " + datapath 
                                    +  "gene.tre | grep extra | cut -d ' ' -f 6" );    
   //cout << ls << endl;
   return ls;
}
string InferSpeciesTree::getMpestScoreList(string varFile, int popSize)
{
    string ls = "";
    string cmd = "lib/mpest/" + os + "/mpest " + datapath + "control_score " + varFile;
    for(int i=0; i<popSize; i++)
    {
        ls += GetStdoutFromCommand(cmd + " " + std::to_string(i));
        //ls += "\n"; 
    }
    return ls;
}
string InferSpeciesTree::getStelarScoreList(string varFile, int popSize)
{
    //cout << "Getting Astral Score"<< endl;
    string ls = GetStdoutFromCommand("java -cp lib/STELAR/main.jar:lib/STELAR/STELAR_MAN.jar phylonet.coalescent.MGDInference_DP -st " + varFile 
                                    + " -i " + datapath +  "gene.tre");
    //cout << ls << endl;
    return ls;
}

void InferSpeciesTree::evaluate(SolutionSet *pop, int gen)
{
    setBranchLength(pop, 1.0);
    varFile_ = datapath + "tmp";
    if(!fileExistsTest(varFile_))
    {
        GetStdoutFromCommand("mkdir " + varFile_);
    }
    varFile_ += "/"+ to_string(timestamp_) + to_string(threadId_);
    pop->printVariablesToFile(varFile_);
    for(int i=0; i<selectedObjectives.size(); i++)
    {
        int objId = selectedObjectives[i];
        string scoreList = (this->*(getScoreFunctions[objId]))(varFile_, pop->size());
        stringstream ss(scoreList);
        string to;
        int solId=0;
        while(std::getline(ss,to,'\n')){
          double value = atof(to.c_str());
          pop->get(solId)->setObjective(i, value * objNegIfMax[objId]);
          //cout << pop->get(solId)->getObjective(i) << endl;
          updateReference(pop->get(solId), i);
          solId++;
        }
    }
    //cout << ls << endl;     
}
void InferSpeciesTree::evaluateFitness(SolutionSet *pop)
{
    for(int i = 0; i < pop->size(); i++) //for each solution
    {
        Solution * sol = pop->get(i);
        double fitness = 0;
        for(int n = 0; n < getNumberOfObjectives(); n++)
        {
            fitness += ( (sol->getObjective(n) - getMinObjective(n)) / (getMaxObjective(n) - getMinObjective(n)) );      
        }
        sol->setFitness(fitness);
    }
}
void InferSpeciesTree::updateReference(Solution * individual) {
  for (int n = 0; n < getNumberOfObjectives(); n++) {
      updateReference(individual, n);
  }
}
void InferSpeciesTree::updateReference(Solution * individual, int n) {
    if (individual->getObjective(n) < objMin[n]) 
    {
        objMin[n] = individual->getObjective(n);
    }
    else if(individual->getObjective(n) > objMax[n])
    {
        objMax[n] = individual->getObjective(n);
    }
  
}
bool InferSpeciesTree::matchObjectiveValues(Solution * one, Solution * two )
{
    //bool match = true;
    for(int i=0; i<numberOfObjectives_; i++)
    {
        if(one->getObjective(i) != two->getObjective(i)) //fabs(one->getObjective(i) - two->getObjective(i)) > 0.00001
        {
            return false;
        }
    }
    return true;
}

InferSpeciesTree::~InferSpeciesTree() {

    //cout<< "=====The END=====";
    string cmd = "rm "+ varFile_;
    //cout<<cmd;
    GetStdoutFromCommand(cmd);
    //delete newick;



    //if (ParsimonyEvaluator!=NULL)   delete ParsimonyEvaluator;
    //if (LikelihoodEvaluator!=NULL)  delete LikelihoodEvaluator;

    delete solutionType_;

    for (int i = 0; i < precomputedTrees.size(); i++) {
        //TreeTemplateTools::deleteSubtree(*trees[i],trees[i]->getRootId());
        delete precomputedTrees[i];
    }

    //if(printtrace)  CloseScores();


} // ~InferSpeciesTree


//void  InferSpeciesTree::fillupNewPopulationUsingOld(SolutionSet * newPop, SolutionSet * oldPop)
//{
//    //newPop = new SolutionSet(size);
//    map<string, void *> parameters;
//    double prob = 0.8; int numDes = 1.0;
//    parameters["probability"] = &prob;
//    parameters["numDescendientes"] = &numDes;
//    TreeCrossover * crossover = new TreeCrossover(parameters);
//
//    prob = 1.0;
//    parameters.clear();
//    parameters["probability"] = &prob;
//
//    string method[] = {"NNI", "SPR", "TBR"};
//    vector<Mutation *> mutList1, mutList2;
//
//    parameters["metodo"] = &method[0];
//    Mutation * NNI = new PhylogeneticMutation(parameters);
//    parameters["metodo"] = &method[1];
//    Mutation * SPR = new PhylogeneticMutation(parameters);
//    parameters["metodo"] = &method[2];
//    Mutation * TBR = new PhylogeneticMutation(parameters);
//
//
//    mutList1.push_back(NNI);
//    mutList1.push_back(SPR);
//    mutList1.push_back(TBR);
//    parameters.clear();
//    parameters["probability"] = &prob;
//    parameters["mutationList"] = &mutList1;
//    Mutation * ShufMut = new ShuffledMutation(parameters);
//
//    mutList2.push_back(NNI);
//    mutList2.push_back(SPR);
//    mutList2.push_back(TBR);
//    //mutList2.push_back(ShufMut);
//    parameters.clear();
//    prob = 1.0;
//    parameters["probability"] = &prob;
//    parameters["mutationList"] = &mutList2;
//    Mutation * mulMut = new MultipleRandomMutation(parameters);
//    
//    Solution ** parents = new Solution*[2];
//    int parent1, parent2;
//    //vector<Solution> children, parents;
//    while (newPop->size() < newPop->getMaxSize()) {
//
//        parent1 = PseudoRandom::randInt(0, oldPop->size() - 1);
//        do {
//            parent2 = PseudoRandom::randInt(0, oldPop->size() - 1);
//        } while (parent1 == parent2);
//        
//        parents[0] = oldPop->get(parent1);
//        parents[1] = oldPop->get(parent2);
//        
//        //Solution * sol = new Solution(parents[0]);
//        
//        Solution * offSpring = (Solution *) (crossover->execute(parents));
//        if(isMultifurcating(offSpring))
//        {
//            #ifdef MAN_DEBUG
//            cout<<"TreeCrossover returned a non-binary tree. "<<endl;
//            #endif
//            continue;
//        }
//        mulMut->execute(offSpring);   
//        if(isMultifurcating(offSpring))
//        {
//            #ifdef MAN_DEBUG
//            cout<<"When non-binary, then pllvalidator: "<<PLLisTreeValidate(offSpring)<<endl;
//            #endif
//            continue;
//        }
//        if( getNumberOfLeaves(offSpring) != this->numberOfTaxa_)
//        {   
//            #ifdef MAN_DEBUG
//            cout<<"When less taxa, then pllvalidator: "<<PLLisTreeValidate(offSpring)<<endl;
//            #endif
//            continue;
//        }
//        newPop->add(offSpring);
//    }
//    delete[] parents;
//    //return newPop;
//}
//SolutionSet * InferSpeciesTree::createInitialPopulationGeneTrees(int size) 
//{
//    SolutionSet * genePop = getSolutionSetFromVarFile(datapath + "gene.tre");
//    SolutionSet * outPop = new SolutionSet(size);
//    fillupNewPopulationUsingOld(outPop, genePop);
//    
//
//    return outPop;
//}
//SolutionSet * InferSpeciesTree::createInitialPopulation(int size) {
//    SolutionSet * pop = new SolutionSet(size);
//    //unordered_set<string> uniqueSolutions;
//for (int i = 0; i < precomputedTrees.size(); i++) {
//        Variable **variables = new Variable*[this->getNumberOfVariables()];
//        variables[0] = new PhyloTree(precomputedTrees[i]);
//        Solution * newSolution = new Solution(this, variables);
//        pop->add(newSolution);
//    }
//    fillupNewPopulationUsingOld(pop, pop);
//    
////    map<string, void *> parameters;
////    double prob = 1.0; int numDes = 1.0;
////    parameters["probability"] = &prob;
////    parameters["numDescendientes"] = &numDes;
////    TreeCrossover * crossover = new TreeCrossover(parameters);
////
////    prob = 1.0;
////    parameters.clear();
////    parameters["probability"] = &prob;
////
////    string method[] = {"NNI", "SPR", "TBR"};
////    vector<Mutation *> mutList1, mutList2;
////
////    parameters["metodo"] = &method[0];
////    Mutation * NNI = new PhylogeneticMutation(parameters);
////    parameters["metodo"] = &method[1];
////    Mutation * SPR = new PhylogeneticMutation(parameters);
////    parameters["metodo"] = &method[2];
////    Mutation * TBR = new PhylogeneticMutation(parameters);
////
////
////    mutList1.push_back(NNI);
////    mutList1.push_back(SPR);
////    mutList1.push_back(TBR);
////    parameters.clear();
////    parameters["probability"] = &prob;
////    parameters["mutationList"] = &mutList1;
////    Mutation * ShufMut = new ShuffledMutation(parameters);
////
////    mutList2.push_back(NNI);
////    mutList2.push_back(SPR);
////    mutList2.push_back(TBR);
////    //mutList2.push_back(ShufMut);
////    parameters.clear();
////    prob = 0.6;
////    parameters["probability"] = &prob;
////    parameters["mutationList"] = &mutList2;
////    Mutation * mulMut = new MultipleRandomMutation(parameters);
////    
////    Solution ** parents = new Solution*[2];
////    int parent1, parent2;
////    //vector<Solution> children, parents;
////    while (pop->size() < size) {
////
////        parent1 = PseudoRandom::randInt(0, pop->size() - 1);
////        do {
////            parent2 = PseudoRandom::randInt(0, pop->size() - 1);
////        } while (parent1 == parent2);
////        
////        parents[0] = pop->get(parent1);
////        parents[1] = pop->get(parent2);
////        
////        //Solution * sol = new Solution(parents[0]);
////        
////        Solution * offSpring = (Solution *) (crossover->execute(parents));
////        if(isMultifurcating(offSpring))
////            continue;
////        mulMut->execute(offSpring);   
////        if( getNumberOfLeaves(offSpring) != this->numberOfTaxa_ || isMultifurcating(offSpring))
////        {   
////            #ifdef MAN_DEBUG
////            cout<<"When less taxa, then pllvalidator: "<<PLLisTreeValidate(offSpring)<<endl;
////            #endif
////            continue;
////        }
////        pop->add(offSpring);
////        
////        //delete[] offSpring;
////    }
////     for (int i = 0; i < precomputedTrees.size(); ) {
////        //Solution * sol = new Solution(pop->get(pop->size() - i - 1));
////        parents[0] = pop->get(i);
////        parents[1] = pop->get(pop->size() - i - 1);
////        Solution * offSpring = (Solution *) (crossover->execute(parents));
////        if(isMultifurcating(offSpring))
////        {
////            continue;
////        }
////        //mulMut->execute(sol);
////        pop->replace(i, offSpring);
////        i++;
////    }
////    delete[] parents;
//    return pop;
//}