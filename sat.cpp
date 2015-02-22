using namespace std;

#include <cstring>
#include <cstdlib>
#include <vector>
#include "parser.h"
#include "clause.h"
#include "ValliJGM.h"


// Assignment data structure
vector<vector<int>> _assignments;
// Assignment level is assignments[i]
// The variable assigned at level i is jth of assignments[i][j]

// Variable data structure
// Sorted linked list structure I have...
// Maintains sortedness and only one variable in the structure
Valli<Variable> _variables;
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

enum heuristic {
  // STATIC
  UNSAT = 0,
  SAT = 1
};


void DPLL(vector<Clause *> &set_of_clauses) {
  // do BCP
  UnitPropogate(set_of_clauses);
  if (isSatisfied(set_of_clauses))
    return(SAT); // you have simplified every clause to be "1"
  if (isConflicting(set_of_clauses)) 
    return(UNSAT); // this is a conflict, thhis set of var assignment doesn't satisfy
  // must recurse
  
  // Decision step using branch heuristic
  Heuristically choose an unassigned variable x and heuristically choose a value v
	  
	 // DPLL(set_of_clauses = simplified by setting x=v) == (SAT) 
    if ( DPLL(set_of_clauses) == (SAT) )
      return(SAT);
    else return( DPLL(set_of_clauses = simplified by setting x=-v) );
}

// Think this is right but need to check
bool isSatisfied(vector<Clause *> &set_of_clauses) {
   for (int i = 0, int sz = set_of_clauses.size(); i < sz; i++) {
   	if (clauseSatisfied( set_of_clauses(i) ))
	      return true;
		return false;
	}
}

// Think this is right but need to check
bool clauseSatisfed(Clause *clause) {
	for (int i = 0, int cl_len = clause->numLiterals(); i < cl_len; i++) {
		bool litSign = (clause->literals(i) > 0) ? true : false;
		unsigned varSetting = *_variables.find(clause->literals(i)).getSetting();
      if ( (litSign ^ varSign) == 1) //Need to check this 
         return true;
      return false;
	}
}

// Think this is right but need to check
bool isConflicting(vector<Clause *> &set_of_clauses) {
   for (int i = 0, int sz = set_of_clauses.size(); i < sz; i++) {
   	if (clauseConflicting( set_of_clauses(i) ))
	      return false;
		return true;
	}
}

// Think this is right but need to check
bool clauseConflicting(Clause *clause) {
	for (int i = 0, int cl_len = clause->numLiterals(); i < cl_len; i++) {
		bool litSign = (clause->literals(i) > 0) ? true : false;
		unsigned varSetting = *_variables.find(clause->literals(i)).getSetting();
      if ( (litSign ^ varSign) != 0) //Need to check this 
         return false;
      return true;
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
		Clause cl = Clause(clauses(i));
		set_of_clauses.push_back(cl);
	}
	
   // Solve SAT
   DPLL(set_of_clauses);
	
	// TODO output processing

   return 0;
}
