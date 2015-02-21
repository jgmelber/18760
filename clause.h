#ifndef CLAUSE_H_
#define CLAUSE_H_

#include <stack>
#include <vector>

using std::vector;

class Clause {

  // Variable descriptions:
  // learned: is used to mark if the clause is learned
  // activity: how often the clause has been in involved in conflicts
  // literals: the vector of literals 
  bool learned;
  int activity;
  //vector<Literal> literals;
  vector<int> literals;

public:
     
  // This is the constructor for the Clause.
  Clause() 
	  : learned(false)
     , activity(0)
	  , literals(vector<int>())
  { }
  
  explicit Clause(vector<int> literals) 
	  : learned(false)
	  , activity(0)
	  , literals(literals)
  { }
  
  // This is the destructor for the Clause.
  virtual ~Clause() { }
  
  // Will deal with learned, activity later...
  /*void setLearned(bool learned) {
     learned = learned;
  }
  
  bool isLearned() {return learned};*/
  
  //REQ: literal cannot be 0
  void addLiteral(int literal) {
  	  //literals.push_back(Literal(literal));
  	  literals.push_back(literal);
  }
  
  int numLiterals() {
	  return literals.size();
  }
  
  vector<int> literals() {
	  return literals;
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
	bool setting;
	int decisionLevel;
	//vector<int> watchedIn;
	
	public:
		Variable () 
			: value(0)
			, setting(true)
			, decisionLevel(-1)
		{ }
		
		explicit Variable(int value)
			: value(value)
			, setting(true)
			, decisionLevel(-1)
		{ }
		
		virtual ~Variable ();
		
		inline bool operator< (const Variable& lhs, const Variable& rhs) { 
		   return lhs.getValue() < rhs.getValue();
		}
		
		int getValue() {
			return value;
		}
		
		bool getSetting() {
			return setting;
		}
		
		void setSetting(bool setting) {
			setting = setting;
		}
		
		int getDecisionLevel() {
			return decisionLevel;
		}
		
		void setDecisionLevel(int decisionLevel) {
			decisionLevel = decisionLevel;
			// Maybe need to set setting here also?
		}
		

};

#endif
