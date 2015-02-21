/* Project #1 / Assignment #7
 * File:   ValliJGM.h
 * Author: Joseph G. Melber
 * UBIT: jgmelber
 *
 * Created on March 19, 2013
 */

#ifndef VALLI_H_
#define VALLI_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;     //From now on we rule out "using namespace std;"
using std::iostream;   //in library files---only clients may do it.
using std::ostringstream;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;


template <typename I>  //REQ: I has I() and operator<
class Valli;


template <typename I>
class DNode {
public:
   /** A pointer to the list I belong to---added by KWR.
       [Not needed or even good for your node class, *yes* should do for
       your iterator class.]
   */
   Valli<I>* myList;

   // A copy of the data   (KWR: we might use pointers to avoid copying)
   I data;

   // Pointers to previous and next DNode (KWR: note different order from text)
   DNode<I>* prev;
   DNode<I>* next;

   DNode(Valli<I>* myList, const I& item, DNode<I>* prev = NULL,
                                         DNode<I>* next = NULL)
    : myList(myList), data(item), next(next), prev(prev)
   { }
};


template <typename I>
class Valli {
   size_t ratio;
   size_t numItems;
   vector<DNode<I>*> milepost;
   DNode<I>* head; //CLASS INV: a "real" node
   DNode<I>* tail; //CLASS INV: a dummy node

   //CLASS INV: head always points to first node, tail to dummy node
   //CLASS INV: Empty list exactly when head == tail == the dummy node
   //CLASS INV: tail never changes; we only insert before it.
   //CLASS INV: items are insterted to maintain sortedness of the data structure
 public: 
   Valli<I>()
	 : head(new DNode<I>(this, I(), NULL, NULL))
	 , tail(head)
     , ratio(20)
	 , numItems(0)
	 , milepost(vector<DNode<I>*>())
   {
      milepost.push_back(head);
      milepost.push_back(tail);
   }

   explicit Valli<I>(size_t ratio)
	 : head(new DNode<I>(this, I(), NULL, NULL))
	 , tail(head)
     , ratio(ratio)
	 , numItems(0)
	 , milepost(vector<DNode<I>*>())
   {
      milepost.push_back(head);
      milepost.push_back(tail);
   }

   virtual ~Valli<I>() {
      //cerr << "This list will self-destruct in 5 nanoseconds..." << endl;
      DNode<I>* current = tail;
      while (current != head) {
         //cerr << "Deleting: " << current->data << "; ";
         current = current->prev;
         delete(current->next);
      }
      //cerr << "Deleting: " << current->data << "; done." << endl;
      delete(current);  //i.e. delete(head);
   }

   class iterator {
      friend class Valli<I>;
      friend class DNode<I>;

      DNode<I>* curr;

      explicit iterator(DNode<I>* given) : curr(given) { }

      public:
      //default copy-ctor and operator= are OK, no destructor needed either

      I& operator*() const { return curr->data; }  //allows assignment
      //WARNING, by allowing assgt this allows breaking Valli's sortedness INV

      //REQ: curr != tail, else may bomb
      //Alternative conventions are tail->next == tail, or == head!
      iterator& operator++() {
         curr = curr->next;
         return *this;   //reference to self is returned
      }

      //REQ: curr != tail, else may bomb
      iterator operator++(int x) {
         iterator oldMe = *this;  //uses copy ctor
         curr = curr->next;
         return oldMe;   //COPY of old self is returned
      }

      bool operator==(const iterator& other) const {
         return curr == other.curr;
      }
      bool operator!=(const iterator& other) const {
         return curr != other.curr;
      }
   };

   iterator begin() const { return iterator(head); }
   iterator end() const { return iterator(tail); }
   size_t size() const { return numItems; }
   bool empty() const { return numItems == 0; } 

   iterator insert(const I& item) {
      iterator itr = begin();
	  if (item < *begin()) { //if item is new head
		  itr = begin();
	  } else { itr = leftPost(milepost,item); }
	  while (itr != end() && *itr < item) { itr++; } //linsearch to find item
      //WHILE LOOP ENS: *itr >= item
      if(*itr != item) { insert(itr,item); } //private version
	  if (tail->next != NULL) {
         cerr << "Uh-oh..." << endl;
      }
      //ENS: first node in mileposts is the head node
	  if(iterator(milepost.at(0)) != begin()) { refresh(); }
      //refresh if size >= 2 * ratio * number of mileposts
      else if(size() >= 2*ratio*milepost.size()) { refresh(); }
	  DNode<I>* prev = itr.curr->prev;
      return iterator(prev);
   }
   
   iterator find(const I& item) const {
	  //Reuse of binary search
	  size_t left = 0;
      size_t right = milepost.size();
      //INV: left's word <= word < right's word, or right == end.
      while (right > left + 1) {
         size_t mid = (right + left)/2;  //integer division
	     //INV: mid < right, so mid is always a valid location
	     if (item < milepost.at(mid)->data || milepost.at(mid)->data == I()) {
		    right = mid;
	     } else {
		    left = mid;
	     }
      }
	  iterator itl = iterator(milepost.at(left));
	  iterator itr = iterator(milepost.at(left + 1));
	  while (itl != itr) {
	     if( *itl == item){
			 return itl;
		 }
		 itl++;
	  }
	  //while (itr != end() && *itr < item) { itr++; } //linsearch to find item
      //   if(*itr != item) { return end(); }
	  return end();
   }

   //REQ: itr to be a valid iterator to an item in the list
   void erase(iterator itr) {
	  //Reuse of binary search
	  size_t left = 0;
      size_t right = milepost.size();
      //INV: left's word <= word < right's word, or right == end.
      while (right > left + 1) {
         size_t mid = (right + left)/2;  //integer division
	     //INV: mid < right, so mid is always a valid location
	     if (*itr < milepost.at(mid)->data || milepost.at(mid)->data == I()) {
		    right = mid;
	     } else {
		    left = mid;
	     }
      }
	  //if itr is one of the mileposts -> move the milepost forward
      if( itr == iterator(milepost.at(left))) {
		  milepost.at(left) = milepost.at(left)->next;
	  }
	  if (itr.curr == tail) {
      //if (itr == end()) {
         cerr << "Attempt to delete the end." << endl;
         return;
      } //else
      DNode<I>* prev = itr.curr->prev;
      DNode<I>* curr = itr.curr;
      if (curr == head) {
         head = curr->next;
      }
	  if(prev != NULL) prev->next = curr->next;
	  curr->next->prev = prev;	  
	  delete(curr);
      numItems--;
	  //if two posts are equal -> refresh
	  if( milepost.at(left)->data == milepost.at(left + 1)->data) { refresh(); }
   }//ENS: mileposts are never equal to each other or point to a non-existant
    //     node
      
   void refresh() {
	  //cerr << "refreshing mileposts ... " << endl;
      milepost.clear();
	  milepost.push_back(head);
	  iterator it = begin();
	  while(size() > ratio && milepost.size() < (size() / ratio)) {
		 size_t count = 0;
		 while(count < ratio){
		    it++;
		    count++;
		 }
	     milepost.push_back(it.curr);
	  }
	  milepost.push_back(tail);
   }//ENS: mileposts contain pointers to head and tail
    //     mileposts.size() >= ( n / ratio )

   void refresh(size_t newRatio) {
	   //cerr << "refreshing mileposts ... " << endl;
	   ratio = newRatio;
	   refresh();
   }//ENS: mileposts contain pointers to head and tail
    //     mileposts.size() >= ( n / ratio )

   string str() const {
      ostringstream OUT;
      iterator itr = begin();
      while (itr != end()) {
         OUT << *itr << '\n';
         ++itr;
      }
      return OUT.str();
   }

   string toString() const {
      ostringstream OUT;
      iterator itr = begin();
      while (itr != end()) {
         OUT << *itr << " ";
         ++itr;
      }
      return OUT.str();
   }

   string milepostString() const{
	  ostringstream OUT;
      for (int i = 0; i < milepost.size(); i++){
	     OUT << milepost.at(i)->data << '\n';
	  
	  }
      return OUT.str();
   }

/*-------------------------------------------------------------------------
Private Utility Methods.
*///-----------------------------------------------------------------------

 private:
   //REQ: pos should come immediately after newitem
   //     newitem is inserted immediately before pos
   void insert(const iterator& pos, const I& newItem) {
	  numItems++;
	  DNode<I>* newNode = new DNode<I>(this,newItem,pos.curr->prev,pos.curr);
      if (pos.curr->prev != NULL) {   //if we're not inserting a new first node
         pos.curr->prev->next = newNode;
         pos.curr->prev = newNode;  
      } else {
         pos.curr->prev = newNode;
         head = newNode;
      }
   }

   //REQ: word > words.at(0)
   iterator leftPost(const vector<DNode<I>*>& words, const I& word) const {
      size_t left = 0;
      size_t right = words.size();
      //INV: left's word <= word < right's word, or right == end.
      while (right > left + 1) {
         size_t mid = (right + left)/2;  //integer division
	     //INV: mid < right, so mid is always a valid location
	     if (word < words.at(mid)->data || words.at(mid)->data == I()) {
		    right = mid;
	     } else {
		    left = mid;
	     }
      }
      return iterator(words.at(left));
   }//ENS: return an iterator to the milepost to the left of the item 
   //Very important to note that the termination property right == left+1
   //is generally MUCH MORE USEFUL than the form in the text which ends with
   //left == right (or even right < left).  Ending with left == right is a waste!
   //Whereas right == left+1 is really useful for something like Valli where
   //those two pointers "bookend" a *range* in the other data structure.

};
#endif

