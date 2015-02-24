#ifndef SAT_H_
#define SAT_H_

using namespace std;

#include <cstring>
#include <cstdlib>
#include <vector>
#include "parser.h"
#include "clause.h"
#include "ValliJGM.h"

// Ignore this enum, this is possibly for later use
/*enum heuristic {
  // STATIC
  MOM = 0,
  JWS = 1,
  SATZ = 2,
  //DYNAMIC
  DLIS = 3,
};*/

enum outcome {
  // STATIC
  UNSATISFIED = 0,
  SATISFIED = 1
};

class SAT
{	
	// Assignment data structure
	vector<vector<int> > _assignments;
	// Assignment level is assignments[i]
	// The variable assigned at level i is jth of assignments[i][j]

	// Variable data structure
	// Sorted linked list structure I have...
	// Maintains sortedness and only one variable in the structure
	Valli<Variable> _variables;
	// Need this to track whether the variable has been assigned	
	
	int _decisionLevel;
	
public:
	SAT () 
		: _decisionLevel(0) 
		, _variables(Valli<Variable>())
		, _unitAssigned(stack<int>())	
	{ }
	
	virtual ~SAT () { }

	outcome DPLL(vector<Clause *> &set_of_clauses) {
	  /*Valli<Variable>::iterator iter = _variables.begin();
	  Valli<Variable>::iterator end = _variables.end();
	  while (iter != end) {
	  	  cout << (*iter).to_short();
		  iter++;
	  }*/
	  // do BCP
	  //UnitPropogate(set_of_clauses);
	  if (isSatisfied(set_of_clauses)) {
		 cout << "SAT!" << endl; 
	    return (SATISFIED); // you have simplified every clause to be "1"
	  } else if (isConflicting(set_of_clauses)) {
		 cout << "UNSAT!" << endl;
	    return (UNSATISFIED); // this is a conflict, this set of var assignment doesn't satisfy
	  // must recurse
	  } else if (hasUnit(set_of_clauses)) {
		  // get Unit and set appropriately
 		  cout << "UNIT Found!" << endl; 
		  findUnit(set_of_clauses);
	     return DPLL(set_of_clauses);	
	  } else {
	  // Decision step using branch heuristic
	  // Heuristically choose an unassigned variable x and heuristically choose a value v
	 	  Valli<Variable>::iterator variable = decide(set_of_clauses);

	 	  // DPLL(set_of_clauses = simplified by setting x=v) == (SAT)
	     if ( DPLL(set_of_clauses) == (SATISFIED) ) {
		    cout << "SAT!" << endl; 
	       return(SATISFIED);
	     } else { 
	 	  // DPLL(set_of_clauses = simplified by setting x=-v)
	 		  --_decisionLevel;
			  int sign = (*variable).getSetting();
			  (*variable).setSetting( (sign != 1) );
			  cout << "Reversing setting ..." << endl;
			  cout << (*variable).to_string() << endl;
			  return( DPLL(set_of_clauses) );
		  }
	  }
	}

	Valli<Variable>::iterator decide(vector<Clause *> &set_of_clauses) {
		++_decisionLevel;
		/*//JW
		int variable = 0;
		int minScore = 1000000000;
		Valli<Variable>::iterator it = _variables.begin();
	   while (it != _variables.end()) {
			if ((*it).getSetting() == 2){
			   int posScore = (*it).scorePOS();
				int negScore = (*it).scoreNEG();
				if (posScore < minScore) {
					minScore = posScore;
					variable = (*it).getValue();
				}
				if (negScore < minScore) {
					minScore = negScore;
					variable = -(*it).getValue();
				}
			}
			++it;
		}*/
		int variable = 0;
		int minScore = 0;
		Valli<Variable>::iterator it = _variables.begin();
		bool sign = true;
	   while (it != _variables.end()) {
			if ( ((*it).getSetting()) == 2 ) {
	  		   stringstream tt;
	  		   tt << (*it).getValue();
	 		   cout << "Found: " << tt.str() << endl;
				int pos = (*it).scorePOS();
				int neg = (*it).scoreNEG();
				sign = pos > neg;
				break;
			}
			++it;
		} 
 	   (*it).setSetting(sign);
 	   (*it).setDecisionLevel(_decisionLevel);
	   cout << (*it).to_string() << endl;
		return it;
	}

	bool isSatisfied(vector<Clause *> &set_of_clauses) {
	   for (int i = 0, sz = set_of_clauses.size(); i < sz; i++) {
	   	if (!clauseSatisfied( set_of_clauses[i] ))
		      return false;
		}
			return true;
	}

	bool clauseSatisfied(Clause *clause) {
		for (int i = 0, cl_len = clause->numLiterals(); i < cl_len; i++) {
			int lit = clause->literals(i);
			int value = (lit > 0) ? lit : -lit;
			bool litSign = (lit > 0) ? 0 : 1;
			unsigned varSetting = (*_variables.find(Variable(value))).getSetting();
	      if ( (litSign ^ varSetting) == 1) 
	         return true;
		}
	      return false;
	}

	bool isConflicting(vector<Clause *> &set_of_clauses) {
	   for (int i = 0, sz = set_of_clauses.size(); i < sz; i++) {
	   	if (clauseConflicting( set_of_clauses[i] ))
		      return true;
		}
			return false;
	}
 
	bool clauseConflicting(Clause *clause) {
		for (int i = 0, cl_len = clause->numLiterals(); i < cl_len; i++) {
			int lit = clause->literals(i);
			int value = (lit > 0) ? lit : -lit;
			bool litSign = (lit > 0) ? 0 : 1;
			unsigned varSetting = (*_variables.find(Variable(value))).getSetting();
	      if ( (litSign ^ varSetting) != 0) 
	         return false;
		}
	      return true;
	}

	bool hasUnit(vector<Clause *> &set_of_clauses) {
		for (int i = 0, sz = set_of_clauses.size(); i < sz; i++) {
			if (clauseUnit(set_of_clauses[i]))
				return true;
		}
		return false;
	}

	bool clauseUnit(Clause *clause) {
		int unassigned = 0;
		for (int i = 0, sz = clause->numLiterals(); i < sz; i++) {
			int lit = clause->literals(i);
			int value = (lit > 0) ? lit : -lit;
			bool litSign = (lit > 0) ? 0 : 1;
			unsigned varSetting = (*_variables.find(Variable(value))).getSetting();
			unsigned setting = litSign ^ varSetting;
			if (setting == 1) 
				// Satisfied
				return false;
			else if (setting != 0) {
				// Not conflict
				unassigned++;
			}
		}
		return (unassigned == 1);
	}

	void findUnit(vector<Clause *> &set_of_clauses) {
		++_decisionLevel;
		for (int i = 0, sz = set_of_clauses.size(); i < sz; i++) {
			if (clauseUnit(set_of_clauses[i])) {
				findUnitLiteral(set_of_clauses[i]);
				break;
			}
		}
	}

	void findUnitLiteral(Clause *clause) {
		Valli<Variable>::iterator it = _variables.begin();
	   bool sign = true;
		for (int i = 0, sz = clause->numLiterals(); i < sz; i++) {
			int lit = clause->literals(i);
			int value = (lit > 0) ? lit : -lit;
			it = _variables.find(Variable(value));
		   unsigned varSetting = (*it).getSetting();
			if (varSetting == 2) {
				stringstream tt;
				int value = (*it).getValue();
	  		   tt << value;
	 		   cout << "Unit Found: " << tt.str() << endl;
				sign = (lit > 0);
				break;
			}
		}
 	   (*it).setSetting(sign);
 	   (*it).setDecisionLevel(_decisionLevel);
	   cout << (*it).to_string() << endl;
	}

	void fillVariables(vector<Clause *> &set_of_clauses) {
		for (int i = 0, sz = set_of_clauses.size(); i < sz; i++) {
			for (int j = 0, len = set_of_clauses[i]->numLiterals(); j < len; j++) {
				int literal = set_of_clauses[i]->literals(j);
				//cout << literal << endl;
				int variable = (literal > 0) ? literal : -literal;
				if (literal > 0) {
					Valli<Variable>::iterator it = _variables.insert( Variable(variable));
					(*it).push_backPOS(set_of_clauses[i]);
					//cout << (*it).to_string() << endl;
					//cout << set_of_clauses[i]->to_string() << endl << endl;
				} else {
					Valli<Variable>::iterator it = _variables.insert( Variable(variable));
					(*it).push_backNEG(set_of_clauses[i]);
					//cout << (*it).to_string() << endl;
					//cout << set_of_clauses[i]->to_string() << endl << endl;
				}
			}
		}
	}
	
	//This is the first Variable
	Valli<Variable>::iterator getVarStart() {
		return _variables.begin();
	}
	
	//This is the dummy end node
	Valli<Variable>::iterator getVarEnd() {
		return _variables.end();
	}

};



#endif
