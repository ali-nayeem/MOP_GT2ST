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


InferSpeciesTree::InferSpeciesTree(string & _datapath, int _numOfObj) {

    this->datapath =  "data/" +  _datapath + "/";
    numberOfVariables_ = 1;
    numberOfObjectives_ = _numOfObj;
    numberOfConstraints_ = 0;
    problemName_ = "Infer Species Tree: " + _datapath;
    solutionType_ = new PhyloTreeSolutionType(this);
    treeFiles.push_back(datapath + speciesTreeFileName + "_astral");
    treeFiles.push_back(datapath + speciesTreeFileName + "_mpest");
    treeFiles.push_back(datapath + speciesTreeFileName + "_phylonet");
    //treeFiles.push_back(datapath + speciesTreeFileName + "_triplet");
    newick = new Newick;
    timestamp_ = time(0);
    threadId_ = -1;
    readPrecomputedSpeciesTree();
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

void InferSpeciesTree::readPrecomputedSpeciesTree() {
    for (int i = 0; i < treeFiles.size(); i++) {
        
        if (fileExistsTest(treeFiles.at(i))) {
            #ifdef MAN_DEBUG
                cout << "Reading precomputed tree: "<< treeFiles[i] << endl;
            #endif
            precomputedTrees.push_back(new PhyloTree(treeFiles[i]));
            //             Variable **variables = new Variable*[this->getNumberOfVariables()];
            //             variables[0] = new SpeciesTree(treeFiles[i]);
            //             Solution * newSolution = new Solution(this, variables);
            //             cout << variables[0]->toString()<<endl;        
            //cout << precomputedTrees[i]->getName();
            //TreeTemplate<Node> * tree = precomputedTrees[i]->getTree();
            //cout << tree->getNumberOfLeaves() << endl;
            //Node * Nodo2 = precomputedTrees[i]->selectrandomnodeToCross();//selectNodeToCross(tree, tree->getNodesId());
            //Node * padre= Nodo2->getFather();
            //double distancetofather = Nodo2->getDistanceToFather();
            //int PosNodo2= padre->getSonPosition(Nodo2);
            //cout<< distancetofather << ", "<<PosNodo2;
        }        
    }
    this->numberOfTaxa_ = precomputedTrees[0]->getNumberOfLeaves();
//    if(precomputedTrees.size()<2){
//        throw Exception("More than one PreComputedSpeciesTree is needed");
//    }
}
int InferSpeciesTree::getNumberOfTaxa()
{
    return numberOfTaxa_;
}
SolutionSet * InferSpeciesTree::createInitialPopulation(int size) {
    SolutionSet * pop = new SolutionSet(size);
    //unordered_set<string> uniqueSolutions;
for (int i = 0; i < precomputedTrees.size(); i++) {
        Variable **variables = new Variable*[this->getNumberOfVariables()];
        variables[0] = new PhyloTree(precomputedTrees[i]);
        Solution * newSolution = new Solution(this, variables);
        pop->add(newSolution);
    }
    map<string, void *> parameters;
    double prob = 1.0; int numDes = 1.0;
    parameters["probability"] = &prob;
    parameters["numDescendientes"] = &numDes;
    TreeCrossover * crossover = new TreeCrossover(parameters);

    prob = 1.0;
    parameters.clear();
    parameters["probability"] = &prob;

    string method[] = {"NNI", "SPR", "TBR"};
    vector<Mutation *> mutList1, mutList2;

    parameters["metodo"] = &method[0];
    Mutation * NNI = new PhylogeneticMutation(parameters);
    parameters["metodo"] = &method[1];
    Mutation * SPR = new PhylogeneticMutation(parameters);
    parameters["metodo"] = &method[2];
    Mutation * TBR = new PhylogeneticMutation(parameters);


    mutList1.push_back(NNI);
    mutList1.push_back(SPR);
    mutList1.push_back(TBR);
    parameters.clear();
    parameters["probability"] = &prob;
    parameters["mutationList"] = &mutList1;
    Mutation * ShufMut = new ShuffledMutation(parameters);

    mutList2.push_back(NNI);
    mutList2.push_back(SPR);
    mutList2.push_back(TBR);
    //mutList2.push_back(ShufMut);
    parameters.clear();
    prob = 0.6;
    parameters["probability"] = &prob;
    parameters["mutationList"] = &mutList2;
    Mutation * mulMut = new MultipleRandomMutation(parameters);
    
    Solution ** parents = new Solution*[2];
    int parent1, parent2;
    //vector<Solution> children, parents;
    while (pop->size() < size) {

        parent1 = PseudoRandom::randInt(0, pop->size() - 1);
        do {
            parent2 = PseudoRandom::randInt(0, pop->size() - 1);
        } while (parent1 == parent2);
        
        parents[0] = pop->get(parent1);
        parents[1] = pop->get(parent2);
        
        //Solution * sol = new Solution(parents[0]);
        
        Solution * offSpring = (Solution *) (crossover->execute(parents));
        if(isMultifurcating(offSpring))
            continue;
        mulMut->execute(offSpring);   
        if( getNumberOfLeaves(offSpring) != this->numberOfTaxa_ || isMultifurcating(offSpring))
        {   
            #ifdef MAN_DEBUG
            cout<<"When less taxa, then pllvalidator: "<<PLLisTreeValidate(offSpring)<<endl;
            #endif
            continue;
        }
        pop->add(offSpring);
        
        //delete[] offSpring;
    }
     for (int i = 0; i < precomputedTrees.size(); ) {
        //Solution * sol = new Solution(pop->get(pop->size() - i - 1));
        parents[0] = pop->get(i);
        parents[1] = pop->get(pop->size() - i - 1);
        Solution * offSpring = (Solution *) (crossover->execute(parents));
        if(isMultifurcating(offSpring))
        {
            continue;
        }
        //mulMut->execute(sol);
        pop->replace(i, offSpring);
        i++;
    }
    delete[] parents;
    return pop;
}

boolean InferSpeciesTree::PLLisTreeValidate(TreeTemplate<Node> * tree){
  pllNewickTree * newick;
  boolean res=true;
  string treenewick = TreeTemplateTools::treeToParenthesis(*tree) ;
  newick = pllNewickParseString	(treenewick.c_str());
  if (!newick){
      res = false;
  }else {
      if (!pllValidateNewick (newick))  res=false;  //if (!pllValidateNewick (newick))  !pllNewickUnroot(newick)
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

void InferSpeciesTree::evaluate(SolutionSet *pop, int gen)
{
    varFile_ = datapath + "tmp";
    if(!fileExistsTest(varFile_))
    {
        GetStdoutFromCommand("mkdir " + varFile_);
    }
    varFile_ += "/"+ to_string(timestamp_) + to_string(threadId_);
    pop->printVariablesToFile(varFile_);
    for(int objId=0; objId<numberOfObjectives_; objId++)
    {
        string scoreList = (this->*(getScoreFunctions[objId]))(varFile_, pop->size());
        stringstream ss(scoreList);
        string to;
        int solId=0;
        while(std::getline(ss,to,'\n')){
          double value = atof(to.c_str());
          pop->get(solId)->setObjective(objId, value * objNegIfMax[objId]);
          //cout << pop->get(solId)->getObjective(objId) << endl;
          solId++;
        }
    }
    //cout << ls << endl;
    
    
}

InferSpeciesTree::~InferSpeciesTree() {

    //cout<< "=====The END=====";
    string cmd = "rm "+ varFile_;
    //cout<<cmd;
    GetStdoutFromCommand(cmd);
    delete newick;



    //if (ParsimonyEvaluator!=NULL)   delete ParsimonyEvaluator;
    //if (LikelihoodEvaluator!=NULL)  delete LikelihoodEvaluator;

    delete solutionType_;

    for (int i = 0; i < precomputedTrees.size(); i++) {
        //TreeTemplateTools::deleteSubtree(*trees[i],trees[i]->getRootId());
        delete precomputedTrees[i];
    }

    //if(printtrace)  CloseScores();


} // ~InferSpeciesTree