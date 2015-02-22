//Cody Childers
//cchilder@ucsc.edu
//Kyle Sullivan
//kyrsulli@ucsc.edu

#if !defined(_List_H_INCLUDE_)
#define _List_H_INCLUDE_

class Node{
public:
	Node(int data);
	~Node();
	int data;
	Node* next;
	Node* prev;
};
	
class List{
public: 
// the old implementation required this data, so it was faster to make 
// it public than to rewrite all my old code
	Node* front;
	Node* back;
	Node* current;
	int length;
	

/***** Constructor *****/
	List(void);
	
/***** Destructor *****/
	~List(void);
	
/*
*  getFront
*  Returns the value at the front of L.
*  Pre: !isEmpty(L)
*/
	int getFront();
	
/*
*  getBack
*  Returns the value at the back of L.
*  Pre: !isEmpty(L)
*/
	int getBack();

/*
*  getCurrent
*  Returns the value at the current node of L.
*  Pre: !isEmpty(L), !offEnd(L)
*/
	int getCurrent();
	
/*
*  getIndex
*  Returns the index of the current node of L. Returns -1 if current is undefined.
*  Pre: !isEmpty(L)
*/
	int getIndex();

/*
*  offEnd
*  Returns True if current is undefined, otherwise returns false.
*/
	int offEnd();
/*
*  getLength
*  Returns the length of L
*/
	int getLength();

/*
*  isEmpty
*  Returns True if L is empty, otherwise returns 0
*/
	int isEmpty();
	
	
	bool isIn(int v);
/*
*  insertBack
*  Places new data element at the end of L
*  Post: !isEmpty(L)
*/

	void insertBack(int data);

/*
*  insertFront
*  Places new data element at the front of L
*  Post: !isEmpty(L)
*/
	void insertFront(int data);

/*
*  insertBeforeCurrent
*  Places new data element before current element
*  Pre: !isEmpty(L), !offEnd(L)
*/
	void insertBeforeCurrent(int data);

/*
*  insertAfterCurrent
*  Places new data element after current element
*  Pre: !isEmpty(L), !offEnd(L)
*/
	void insertAfterCurrent(int data);
	
/*
*  deleteFront
*  Deletes element at front of L
*  Pre: !isEmpty(L)
*/
	void deleteFront();

/*
*  deleteBack
*  Deletes element at back of L
*  Pre: !isEmpty(L)
*/
	void deleteBack();

/*
*  deleteCurrent
*  Deletes the current element.
*  Pre: !isEmpty(L), !offEnd(L)
*/
	void deleteCurrent();

/*
*  moveNext
*  Moves the current element one step forward in the list.
*  Pre: !isEmpty, !offEnd
*/
	void moveNext();
	
/*
*  movePrev
*  Moves the current element one step back in the list.
*  Pre: !isEmpty, !offEnd
*/
	void movePrev();

/*
*  moveTo
*  Moves the current element to the indicated spot in the List.
*/
	void moveTo(int i);

/*
*  makeEmpty
*  Empties out the list.
*  Post: isEmpty(L)
*/
	void makeEmpty();
	
/*
*  printList
*  Prints data elements in L on a single line to stdout.
*/
	void printList();

/*
*  equals
*  returns 1 if A is identical to B, 0 otherwise
*/
	int equals(List* B);

/*
*  copyList
*  Creates a new List identical to L.
*  Pre: !isEmpty(L)
*/
	List* copyList();
	
private:
	//nada

};
//
//
//End of List class
//
//
#endif
