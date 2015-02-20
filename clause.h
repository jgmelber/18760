#ifndef CLAUSE_H_
#define CLAUSE_H_

#include <stack>
#include <vector>

using std::vector;

class Clause {

  // Variable descriptions:
  // index: is used to determine variable ordering for the BDD.
  // marked: used in BDD traversals (for size and print)
  // label: The name this node shows to the outside world.
  // low: The bdd_node which is the 'low' node
  // high: The bdd_node which is the 'high' node
  bool learned;
  int activity;
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
  	  literals.push_back(literal);
  }
    
    

};
#endif
