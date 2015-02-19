using namespace std;

#include <cstring>
#include <cstdlib>

#include <parser.h>

enum heuristic {
  // STATIC
  MOM = 0,
  JWS = 1,
  SATZ = 2,
  //DYNAMIC
  DLIS = 3,
};


void DPLL(vector<vector<int> > &set_of_clauses) {
  // do BCP
  UnitPropogate(set_of_clauses);
  if (set_of_clauses is all "1" clauses now)
    return(SAT); // you have simplified every clause to be "1"
  if (set_of_clauses contains a clause that evals to "0") 
    return(UNSAT); // this is a conflict, thhis set of var assignment doesn't satisfy
  // must recurse
  Heuristically choose an unassigned variable x and heuristically choose a value v
    if ( DPLL(set_of_clauses = simplified by setting x=v) == SAT )
      return(SAT);
    else return( DPLL(set_of_clauses = simplified by setting x=-v) );
}

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
   
   //Solve SAT
   UnitPropogate(clauses);
   DPLL(clauses);

   return 0;
}
