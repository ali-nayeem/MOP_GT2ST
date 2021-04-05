#ifndef GEN_MOEAD_ST_H
#define GEN_MOEAD_ST_H

#include <Algorithm.h>
#include "Checkpoint.h"
#include "alg_reference_point.h"
#include "alg_environmental_selection.h"
#include "aux_math.h"


class Gen_MOEAD_ST : public Algorithm{
public:
    Gen_MOEAD_ST(Problem * problem); //, Checkpoint *checkpoint
    SolutionSet * execute();
    
    //MOEAD_ST(const MOEAD_ST& orig);
    //virtual ~MOEAD_ST();
protected:
    Checkpoint *checkpoint_;   
    std::vector<std::size_t> obj_division_p_; 
};

#endif /* G-MOEAD_ST_H */