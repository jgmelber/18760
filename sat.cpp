using namespace std;

#include <cstring>
#include <cstdlib>
#include <vector>
#include "parser.h"
#include "clause.h"


// Assignment data structure
vector<vector<int>> _assignments;
// Assignment level is assignments[i]
// The variable assigned at level i is jth of assignments[i][j]

// Variable data structure
// Maybe this should be the sorted linked list structure I have...
vector<Variable *> _variables;
// Need this to track whether the variable has been assigned


// Ignore this enum, this is possibly for later use
/*enum heuristic {
  // STATIC
  MOM = 0,
  JWS = 1,
  SATZ = 2,
  //DYNAMIC
  DLIS = 3,
};*/


void DPLL(vector<Clause *> &set_of_clauses) {
  // do BCP
  UnitPropogate(set_of_clauses);
  if (set_of_clauses is all "1" clauses now)
    return(SAT); // you have simplified every clause to be "1"
  if (set_of_clauses contains a clause that evals to "0") 
    return(UNSAT); // this is a conflict, thhis set of var assignment doesn't satisfy
  // must recurse
  
  // Decision step using branch heuristic
  Heuristically choose an unassigned variable x and heuristically choose a value v
    if ( DPLL(set_of_clauses = simplified by setting x=v) == SAT )
      return(SAT);
    else return( DPLL(set_of_clauses = simplified by setting x=-v) );
}

bool isSatisfied(vector<Clause *> &set_of_clauses) {
   for (int i = 0, int sz = set_of_clauses.size(); i < sz; i++) {
   	if (clauseSatisfied( set_of_clauses(i) ))
	      return true;
		return false;
	}
}

bool clauseSatisfed(Clause *clause) {
	for (int i = 0, int cl_len = clause->numLiterals(); i < cl_len; i++) {
      if ( clause->literals(i) ) //Need to check this against variable set to 
         return true;
      return false;
	}
}

// This is the BCP proceedure
void UnitPropogate(vector<vector<int> > &set_of_clauses) {
  while (set_of_clauses contains a unit clause due to literal L) {
    Simplify set_of_clauses by setting variable for L to its required value in all clauses
  }
}


int main(int argc, char** argv) {
   if (argc == 1) {
      cout << "Usage: CNF file" << endl;
      return 1;
   }
   //This constructs the clauses vector and maxVarIndex int
   vector<vector<int> > clauses;
   int maxVarIndex = 0;
   parse_DIMACS_CNF(clauses, maxVarIndex, argv[1]);
   // You can refer to the ith clause appearing in the CNF file using the
   // expression `clauses[i]'.  The jth literal of `clauses[i]' can be
   // referred to using `clauses[i][j]'.  The expression `clauses.size()'
   // tells you the number of clauses in the benchmark.
   
	// Insert into our data structure
	vector<Clause *> set_of_clauses;
	// Clause is a vector of literals
	// Literals are integers either positive or negative
	// TODO maybe literals need more information to tell if assigned or not?
	for (int i = 0, int sz = clauses.size(); i < sz; i++) {
		Clause *cl = new Clause(clauses(i));
		set_of_clauses.push_back(cl);
	}
	
   // Solve SAT
   DPLL(set_of_clauses);
	
	// TODO output processing

   return 0;
}
