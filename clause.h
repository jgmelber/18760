#ifndef CLAUSE_H_
#define CLAUSE_H_

#include <stack>
#include <vector>
#include <cmath>

using std::vector;

class Clause {

  // Variable descriptions:
  // learned: is used to mark if the clause is learned
  // activity: how often the clause has been in involved in conflicts
  // literals: the vector of literals 
  bool learned;
  int activity;
  //vector<Literal> literals;
  vector<int> literalVec;

public:
     
  // This is the constructor for the Clause.
  Clause() 
	  : learned(false)
     , activity(0)
	  , literalVec(vector<int>())
  { }
  
  explicit Clause(vector<int> literals) 
	  : learned(false)
	  , activity(0)
	  , literalVec(literals)
  { }
  
  // This is the destructor for the Clause.
  virtual ~Clause () { }
  
  // Will deal with learned, activity later...
  /*void setLearned(bool learned) {
     learned = learned;
  }
  
  bool isLearned() {return learned};*/
  
  //REQ: literal cannot be 0
  void addLiteral(int literal) {
  	  //literals.push_back(Literal(literal));
  	  literalVec.push_back(literal);
  }
  
  int numLiterals() {
	  return literalVec.size();
  }
  
  int literals(int i) {
	  return literalVec[i];
  }
  
  string to_string() {
	  string ret = "{";
	  for (int i = 0, sz = literalVec.size(); i < sz; i++) {
		  stringstream out;
		  out << literalVec[i];
		  ret = ret + out.str() + " ";
	  }
     return ret + "}";
  }  
    

};

/*
// Will use later ?
class Literal
{
	int value; // The value of the literal an integer
	bool watched; // Used to mark if the literal is a watched variable
	bool direction; //Not sure if used yet
	
	public:
		Literal ()
			: value(0)
			, watched(false) 
		{ }
		
		explicit Literal(int value) 
			: value(value) 
		{ }
		
		virtual ~Literal ();
		
		bool isSigned() {
			return (value > 0) ? true : false;
		}
		
		int variable() {
			return (value > 0) ? value : -value;
		}
		
		bool isWatched() {
			return watched;
		}
		
		void setWatched(bool watched) {
			watched = watched;
		}

};*/

// Need this to track whether the variable has been assigned
class Variable
{
	int value;
	unsigned setting; // Takes values true (1), false (0), and unassigned (2)
	int decisionLevel;
	vector<Clause *> POS;
	vector<Clause *> NEG;
	
	public:
		Variable () 
			: value(0)
			, setting(2)
			, decisionLevel(-1)
			, POS(vector<Clause *>())
			, NEG(vector<Clause *>())
		{ }
		
		explicit Variable(int value)
			: value(value)
			, setting(2)
			, decisionLevel(-1)
			, POS(vector<Clause *>())
			, NEG(vector<Clause *>())
		{ }
		
		virtual ~Variable () { }
		
		bool operator<(const Variable& rhs) const {
			return value < rhs.getValue();
		}
		
		bool operator==(const Variable& rhs) const
		{
			return value == rhs.getValue();
		}
		
		bool operator!=(const Variable& rhs) const
		{
			return value != rhs.getValue();
		}
		
		int getValue() const {
			return value;
		}
		
		int getSetting() {
			return setting;
		}
		
		void setSetting(unsigned sett) {
			setting = sett;
		}
		
		void push_backPOS(Clause *clause) {
			POS.push_back(clause);
		}
		
		void push_backNEG(Clause *clause) {
			NEG.push_back(clause);
		}
		
		int scorePOS() {
			int sum = 0;
		   for (unsigned i = 0; i < POS.size(); i++){
		   	sum += 1 / pow( 2, (POS[i]->numLiterals()) );
		   }
			return POS.size();	
		}
		
		int scoreNEG() {
			int sum = 0;
		   for (unsigned i = 0; i < NEG.size(); i++){
		   	sum += 1 / pow( 2, (NEG[i]->numLiterals()) );
		   }
			return NEG.size();	
		}
		
		int getDecisionLevel() {
			return decisionLevel;
		}
		
		void setDecisionLevel(int dLevel) {
			decisionLevel = dLevel;
			// Maybe need to set setting here also?
			if (decisionLevel == -1) {
				setting = 2;
			}
		}
		
	   string to_string() {
	 	  string ret = "Variable: ";
		  stringstream tt;
		  tt << value;
		  stringstream set;
		  set << setting;
 		  ret = ret + tt.str() + " Set: " + set.str() + " POS: ";
	 	  for (int i = 0, sz = POS.size(); i < sz; i++) {
	 		  ret = ret + POS[i]->to_string() + " ";
	 	  }
		  ret = ret + "NEG: ";
	 	  for (int i = 0, sz = NEG.size(); i < sz; i++) {
	 		  ret = ret + NEG[i]->to_string() + " ";
	 	  }
	      return ret + "\n";
	   }  
		
		string to_short() {
	 	  string ret = "Variable: ";
		  stringstream tt;
		  tt << value;
		  stringstream set;
		  set << setting;
 		  ret = ret + tt.str() + " Set: " + set.str();
	     return ret + "\n";
	   }  

};

#endif
