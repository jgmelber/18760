using namespace std;

#include <cstring>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <sstream>
#include "parser.h"
#include "clause.h"
#include "ValliJGM.h"
#include "sat.h"


int main(int argc, char** argv) {
   if (argc == 1) {
      cout << "Usage: CNF file" << endl;
      return 1;
   }
   //This constructs the clauses vector and maxVarIndex int
   vector<vector<int> > clauses;
   int maxVarIndex = 0;
	// cout << argv[1] << endl;
   parse_DIMACS_CNF(clauses, maxVarIndex, argv[1]);
   // You can refer to the ith clause appearing in the CNF file using the
   // expression `clauses[i]'.  The jth literal of `clauses[i]' can be
   // referred to using `clauses[i][j]'.  The expression `clauses.size()'
   // tells you the number of clauses in the benchmark.
   
	// Insert into our data structure
	vector<Clause *> set_of_clauses;
	// Clause is a vector of literals
	// Literals are integers either positive or negative
	for (int i = 0, sz = clauses.size(); i < sz; i++) {
		Clause *cl = new Clause(clauses[i]);
		set_of_clauses.push_back(cl);
	}

	cout << "*******************************************************************" << endl;
	cout << "************************     STARTING     *************************" << endl;
	cout << "*******************************************************************" << endl;
	
	SAT mainSAT;
	
	mainSAT.fillVariables(set_of_clauses);
	
   // Solve SAT
   outcome finish = mainSAT.DPLL(set_of_clauses);
	
	// output processing
	ofstream outfile;
	outfile.open ("output.txt");
	outfile << "c The SAT Solution is:\n";
	if (finish == SATISFIED) {
		outfile << "s SATISFIABLE\n";
		Valli<Variable>::iterator itr = mainSAT.getVarStart();
		Valli<Variable>::iterator end = mainSAT.getVarEnd();
		string varString = "v "; 
		while (itr != end) {
			unsigned var = (*itr).getValue();
			int varS = ((*itr).getSetting() == 0) ? -var : var;
								stringstream out;
								out << varS;
                        varString = varString + out.str() + " ";
			++itr;
		}
		outfile << varString << "\n";
	   outfile.close();
		return 10;
	} else if (finish == UNSATISFIED){
		outfile << "s UNSATISFIABLE\n";
	   outfile.close();
		return 20;
	} else {
		outfile << "s UNKNOWN\n";
	}
	outfile.close();
   return 0;
}
