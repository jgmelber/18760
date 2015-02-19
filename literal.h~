#ifndef CLAUSE_H_
#define CLAUSE_H_

class Clause {

  // Variable descriptions:
  // index: is used to determine variable ordering for the BDD.
  // marked: used in BDD traversals (for size and print)
  // label: The name this node shows to the outside world.
  // low: The bdd_node which is the 'low' node
  // high: The bdd_node which is the 'high' node
  int index;
  int marked;
  char label[100];
  bdd_node* low;
  bdd_node* high;

public:
     
  // This is the constructor for the bdd_node.
  bdd_node(int indexIN, char* labelIN, bdd_node* lowIN, 
	   bdd_node* highIN) {
    index = indexIN;
    strcpy(label, labelIN);
    low = lowIN;
    high = highIN;
    marked = false;
  }
    
  // Returns the index of the current node.
  inline int getIndex() { return index; }
    
  // Returns the 'low' BDD of this node.
  inline bdd_node* getLow() { return low; }
    
  // Returns the 'high' BDD of this node
  inline bdd_node* getHigh() { return high; }
    
  // Returns the label of this node.
  inline char* getLabel() { return label; }   

};
#endif
