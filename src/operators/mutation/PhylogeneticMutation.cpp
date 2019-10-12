/** 
 * MO-Phylogenetics (version 1.0.0) a software tool for multi-objective 
 * phylogenetic inference. This software integrates features 
 * of the jMetalCpp, Bio++ and PLL frameworks.
 * 
 * Copyright (C) 2017 Cristian Zambrano-Vega, Antonio J. Nebro.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For any other enquiries send an Email to Cristian Zambrano
 * czambrano@uteq.edu.ec
 *
 * When publishing work that uses this software please cite us.
 * 
 * @file PhylogeneticMutation.cpp
 */

#include <PhylogeneticMutation.h>


//#include <Bpp/Numeric/Random/RandomTools.h>
void printTree(TreeTemplate<Node> * tree);

/**
 * Constructor
 * Creates a new instance of the PhylogeneticMutation  operator
 */
PhylogeneticMutation::PhylogeneticMutation(map<string, void *> parameters)
: Mutation(parameters) {

   mutationProbability_=0;  Metodo="NNI";
    
   if (parameters["metodo"] != NULL)              Metodo = *(string *) parameters["metodo"];
   if (parameters["probability"] != NULL)         mutationProbability_ = *(double *) parameters["probability"];
   
   //random_gsl = gsl_rng_alloc(gsl_rng_default);
   //randomgsl_alpha = 500;

   
} // PhylogeneticMutation



void PhylogeneticMutation::printParameters(){
   
   cout << "************** Parameters of the Mutation Operator  ************ " << endl;
   cout << "Mutation Method: " <<  Metodo << endl;
   cout << "mutationProbability: " << mutationProbability_ << endl;
  
   cout << endl;
}

/**
 * Destructor
 */
PhylogeneticMutation::~PhylogeneticMutation() { 

} // ~PhylogeneticMutation

string PhylogeneticMutation::getName()
{
    return Metodo;
}

/**
 * Perform the mutation operation
 * @param probability Mutation probability
 * @param solution The solution to mutate
 */
void * PhylogeneticMutation::doMutation(double mutationProbability_, Solution *solution) {
    
    if ( PseudoRandom::randDouble() <= mutationProbability_) {
        
                if(Metodo=="SPR") 
                    SPR(solution); 
                else if(Metodo=="TBR") 
                    TBR(solution);
                else
                {
                    if(!NNI(solution))
                    {
                        modifiedNNI(solution);
                    }
                }
                
    }

} // doMutation

void PhylogeneticMutation::TBR(Solution * solution){
    //vector<int> nodosIDs;
    vector<Node *> nodeVec;
    Variable **variables = solution->getDecisionVariables();
    PhyloTree * Pt = (PhyloTree*) variables[0];
    TreeTemplate<Node> * tree = Pt->getTree();
//    int NextIDNode = tree->getNextId();
//    Node * newNode = new Node(NextIDNode++);
//    Node * node17 = tree->getNode(17);
//    int pos = tree->getNode(18)->getSonPosition(node17);
//    tree->getNode(18)->setSon(pos, newNode);
//    //newNode->addSon(tree->getNode(18));
//    newNode->addSon(node17);
//    tree->rootAt(newNode);
//    cout<<Pt->toString()<<endl;
//    printTree(tree);
//    exit(0);
    Node * nodo;
    Node * nodoi;
    Node * nodoj;
    Node * nodoFather;
    Node * nodoSubTree;

    vector<int> nodosIDs = tree->getNodesId();
    do{
              do{
                      nodo = tree->getNode(RandomTools::pickOne(nodosIDs,true, *PseudoRandom::getRndFactory()));
              }while(nodo->isLeaf() || nodo->getNumberOfSons() < 2 || !nodo->hasFather()); //reject if (leaf || root || )

              if (RandomTools::flipCoin(*PseudoRandom::getRndFactory())) {
                      nodoi= nodo->getSon(0); nodoj= nodo->getSon(1);
              }else{
                      nodoi= nodo->getSon(1); nodoj= nodo->getSon(0);
              }

     }while(nodoi->isLeaf());
     #ifdef MAN_DEBUG
        cout << "node: " << nodo->getId()<<", nodei:"<<nodoi->getId()<<", nodej:"<<nodoj->getId()<<endl ;
     #endif 
     TreeTemplate<Node> * subtree = new TreeTemplate<Node>(nodoi);
     //subtree->resetNodesId();
     nodeVec = subtree->getNodes();
     do{
            nodoSubTree = RandomTools::pickOne(nodeVec,true, *PseudoRandom::getRndFactory());
     }while(nodoSubTree->isLeaf());
     
     if(nodoi->getId() != nodoSubTree->getId())
     {
        //int NextIDNode = subtree->getNextId();
        Node * newNode = new Node(); //NextIDNode++
        Node * Father = nodoSubTree->getFather();
        int pos = Father->getSonPosition(nodoSubTree);
        Father->setSon(pos, newNode);
        //newNode->addSon(tree->getNode(18));
        newNode->addSon(nodoSubTree);
        subtree->rootAt(newNode);
        //nodoSubTree = newNode;
     }
     //subtree->rootAt(nodoSubTree->getId());
     //printTree();
     #ifdef MAN_DEBUG
        cout << "Subtree nodei root: " << nodoSubTree->getId()<<endl ;
        cout<<"New subtree: "<<TreeTemplateTools::treeToParenthesis(*subtree)<<endl;
     #endif 
     //nodo->removeSon(nodoj);
     //nodo->removeSon(nodoi);
     //Node * nuevonodo = nodo;
     nodoFather = nodo->getFather();
     nodoFather->setSon(nodoFather->getSonPosition(nodo),nodoj);
     //tree->resetNodesId();
     nodeVec = tree->getNodes();
     do{
            nodo = RandomTools::pickOne(nodeVec,true, *PseudoRandom::getRndFactory());
     }while(nodo->isLeaf());
      #ifdef MAN_DEBUG
        cout << "New node as adding point: " << nodo->getId()<<endl ;
     #endif
     int posSon;
     if (RandomTools::flipCoin(*PseudoRandom::getRndFactory())) posSon=0; else posSon=1;

     Node * nuevonodo = new Node();
     nuevonodo->addSon(nodo->getSon(posSon));
     nuevonodo->addSon(subtree->getRootNode());
     

     nodo->setSon(posSon,nuevonodo);
     tree->resetNodesId();

}


bool PhylogeneticMutation::NNIValidate(Node * Nodo){

    if (Nodo->getNumberOfSons()>1){
          if (!Nodo->getSon(0)->isLeaf() and !Nodo->getSon(1)->isLeaf()) return true;    
    }
    return false;

}

bool PhylogeneticMutation::NNI(Solution * solution){
    
    //cout << solution->toString();
    PhyloTree * Pt = (PhyloTree*) solution->getDecisionVariables()[0];
    TreeTemplate<Node> * tree = Pt->getTree();
    
    Node * NodoSel;
    vector<Node *> nodes = tree->getInnerNodes();
    for(int i=0; i<nodes.size(); i++)
    {
        if(nodes[i]->getSon(0)->isLeaf() || nodes[i]->getSon(1)->isLeaf())
        {
            nodes.erase(nodes.begin() + i);
            i--;
        }
    }
    if(nodes.size() == 0)
    {
        #ifdef MAN_DEBUG
        cout << "NNI Failed !" << endl ;
        #endif              
        return false;
    }
    int maxCount = nodes.size();
    int count = 0;
    do{
          count++;
          if(count > maxCount)
          {
              #ifdef MAN_DEBUG
              cout << "NNI Failed !" << endl ;
              #endif              
              return false;
          }
          NodoSel = RandomTools::pickOne(nodes, false, *PseudoRandom::getRndFactory());    //nodes[PseudoRandom::randInt(0, nodes.size() - 1)];
    }while(!NNIValidate(NodoSel));
    
    Node * Nodo1;
    Node * Nodo2;
    int Pos1, Pos2;

    Pos1 = PseudoRandom::randInt(0, NodoSel->getSon(0)->getNumberOfSons()-1);
    Pos2 = PseudoRandom::randInt(0, NodoSel->getSon(1)->getNumberOfSons()-1);

    Nodo1=  NodoSel->getSon(0)->getSon(Pos1);
    Nodo2=  NodoSel->getSon(1)->getSon(Pos2);

    NodoSel->getSon(0)->setSon(Pos1,Nodo2);
    NodoSel->getSon(1)->setSon(Pos2,Nodo1);
    return true;

}



//void PhylogeneticMutation::ModificarRamasDistGamma(Solution * solution){
//    
//    /*Also modifies branch lengths in 	order to improve the tree likelihood value.
//    * Some branches, chosen at random, have their
//    * lengths multiplied by a factor obtained from a Γ-distribution (Lewis, 1998)
//    */
//    
//    TreeTemplate<Node> * tree = ((PhyloTree*) solution->getDecisionVariables()[0])->getTree();
//    
//    //if( PseudoRandom::randDouble() > 0.34 ){
//	 double gamma; double distance;
//	 vector<Node *> nodes = tree->getNodes();
//	 for(unsigned int i = 0; i < nodes.size(); i++){
//             if (PseudoRandom::randDouble() <= 0.5) {
//                if (nodes[i]->hasDistanceToFather()){
//			 distance = nodes[i]->getDistanceToFather();
//        		 gamma =  gsl_ran_gamma (random_gsl, randomgsl_alpha, (1.0/randomgsl_alpha) );
//			 if (distance * gamma != 0)	 nodes[i]->setDistanceToFather(distance * gamma);
//		 }
//             }
//	}
//   // }
//    
//}
void printTree(TreeTemplate<Node> * tree)
{
    vector<Node*> nodes1 = tree->getNodes();
    for(int i=0; i<nodes1.size(); i++)
    {
        if(nodes1[i]->isLeaf())
             cout<< "Leaf Node:" << nodes1[i]->getId()<< ", Name:"<<nodes1[i]->getName()<< endl;
        else
        cout<< "Node:" << nodes1[i]->getId()<< ", left child:"<<nodes1[i]->getSon(0)->getId()<< ", right child:"<<nodes1[i]->getSon(1)->getId()<<endl;
    }
//    exit(0);
//    Node* rs = tree->getNode(18)->getSon(1);
//    tree->getNode(18)->removeSon(1);
//    vector<Node*> nodes1 = tree->getNodes();
//    tree->getNode(18)->addSon(1,rs);
//    nodes1 = tree->getNodes();
}
void PhylogeneticMutation::SPR(Solution * solution){
    
    PhyloTree * Pt = (PhyloTree*) solution->getDecisionVariables()[0];
    TreeTemplate<Node> * tree = Pt->getTree();
    int NextIDNode = tree->getNextId();
    bool b;
    Node* Nodo1;
    Node* Nodo2;
    
     vector<Node*> nodes = tree->getNodes();
     do{
        b=true;
        do {
               Nodo1 =  nodes[PseudoRandom::randInt(0, nodes.size()-1)] ; //old: rand()%nodes.size()
               if(Nodo1->hasFather()){
                   if(Nodo1->getFather()->hasFather()) b=false; //ensure Node1 is not root
                }
        }while(b);
     
        Node * Father = Nodo1->getFather();
        int sonPos = Father->getSonPosition(Nodo1);
        Father->removeSon(sonPos);
        vector<Node*> nodes2 = tree->getNodes();
        Nodo2 =  nodes2[PseudoRandom::randInt(0, nodes2.size()-1)]; //old: rand()%nodes.size()
        Father->addSon(sonPos, Nodo1);
        
    }while(!SPRvalide (Nodo1,Nodo2));     
    #ifdef MAN_DEBUG
        cout<<"Node1:" <<Nodo1->getId()<< ", Node2:"<<Nodo2->getId()<<endl;
    #endif
    int PosNodo;
    double distancetofather=0;
    Node * Father;
    Node * Father2;
    Node * GP;
    Node * Brother;
  
    Father=Nodo1->getFather();
    if(Father->getNumberOfSons()==2){ //If you have 2 children Collapse Brother by Father
       PosNodo= Father->getSonPosition(Nodo1);
       Brother = Father->getSon(PosNodo==0?1:0);

       if (Brother->hasDistanceToFather()) {
           distancetofather = Brother->getDistanceToFather();
       }
       
       //Quito the Father without the brother, and place the brother instead of the Father
       Father->removeSon(Brother);
       GP = Father->getFather();
       GP->setSon(GP->getSonPosition(Father),Brother);
       
       if(Father->hasDistanceToFather()) {
               distancetofather+=Father->getDistanceToFather();
       }
      if (Brother->hasDistanceToFather()) {     
        Brother->setDistanceToFather(distancetofather);
      }

     }else{ //If you have more than one brother, Collapse is not done

       PosNodo= Father->getSonPosition(Nodo1);
       Brother = Father->getSon(PosNodo==0?1:0);

       Father->removeSon(Nodo1); //DO NOT remove the NODE just remove it from the Vector of Sons

       Father = new Node(NextIDNode++);
       Father->addSon(Nodo1);
       
     }

     distancetofather=0;
     
     Father2 = Nodo2->getFather();
     
     if(Nodo2->hasDistanceToFather()) {
         distancetofather = Nodo2->getDistanceToFather();
     }

     Father2->setSon(Father2->getSonPosition(Nodo2),Father);
     if(Father->hasDistanceToFather())
     {
         Father->setDistanceToFather(distancetofather/2);
     }
     
     //I add to Node2 as the son of the Father
     Father->addSon(Nodo2);
     if(Nodo2->hasDistanceToFather())
     {
         Nodo2->setDistanceToFather(distancetofather/2);
     }

     /*if(Padre->hasDistanceToFather() and Nodo1->hasDistanceToFather() and Nodo2->hasDistanceToFather())
        if(Padre->getDistanceToFather()>0 and Nodo1->getDistanceToFather()>0 and Nodo2->getDistanceToFather()>0)
        cout << "Ramas a Optimizar dentro SPR " << Padre->getDistanceToFather() << " - " << Nodo1->getDistanceToFather() << " - " << Nodo2->getDistanceToFather() << endl;
     */
}


int  PhylogeneticMutation::SPRvalide (Node* N1, Node* N2) {
    if (!N2->hasFather()) return 0; //N2 is root
    if (N1->getFather()==N2->getFather()) return 0; //N1 and N2 brother
    if (N1->getFather()==N2) return 0;
    if (N1 == N2) return 0;
    
    return 1;
}

bool PhylogeneticMutation::modifiedNNI(Solution * solution){
    PhyloTree * Pt = (PhyloTree*) solution->getDecisionVariables()[0];
    TreeTemplate<Node> * tree = Pt->getTree();
    
    Node * NodoSel;
    vector<Node *> nodes = tree->getInnerNodes();
    int maxCount = nodes.size();
    int count = 0, leaf, non_leaf;
    do{
          count++;
          if(count > maxCount)
          {
              #ifdef MAN_DEBUG
              cout << "modifiedNNI Failed !" << endl ;
              #endif              
              return false;
          }
          NodoSel = RandomTools::pickOne(nodes, false, *PseudoRandom::getRndFactory());    //nodes[PseudoRandom::randInt(0, nodes.size() - 1)];
          if(!NodoSel->getSon(0)->isLeaf() || !NodoSel->getSon(1)->isLeaf())
          {
              if(NodoSel->getSon(0)->isLeaf() || NodoSel->getSon(1)->isLeaf())
              {
                  leaf = (NodoSel->getSon(0)->isLeaf())? 0 : 1;
                  non_leaf = (leaf + 1)%2;
                  break;
              }
          }
          
    }while(true);
    
    Node * Nodo1;
    Node * Nodo2;
    int Pos1, Pos2;

    Pos1 = PseudoRandom::randInt(0, NodoSel->getSon(non_leaf)->getNumberOfSons()-1);
    //Pos2 = PseudoRandom::randInt(0, NodoSel->getSon(1)->getNumberOfSons()-1);

    Nodo1=  NodoSel->getSon(non_leaf)->getSon(Pos1);
    Nodo2=  NodoSel->getSon(leaf);

    NodoSel->getSon(non_leaf)->setSon(Pos1,Nodo2);
    NodoSel->setSon(leaf, Nodo1);
    return true;
}

void * PhylogeneticMutation::execute(void *object) {
  Solution *solution = (Solution *)object;
  // TODO: VALID_TYPES?
  //double probability = *(double *)getParameter("probability");
  doMutation(mutationProbability_, solution);
  return solution;
} // execute
 