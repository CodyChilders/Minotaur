//Cody Childers
//cchilder@ucsc.edu
//Kyle Sullivan
//kyrsulli@ucsc.edu

#include <iostream>
#include <cstdlib> //use the NULL pointer from this
#include "List.h"

using namespace std;

Node:: Node(int inputData){
	data = inputData;
	next = NULL;
	prev = NULL;
}
Node:: ~Node(){
	//no content, it cleans up nicely :D
}

/***** Constructor *****/
List::List(void){
		front = back = current = NULL;
		length = 0;
}
	
/***** Destructor *****/
List::~List(){
	while( !isEmpty() ) { 
		deleteFront(); 
	}
}
	
/*
*  getFront
*  Returns the value at the front of L.
*  Pre: !isEmpty(L)
*/
int List::getFront(){
	if( isEmpty() ){
		cout << "List Error: calling getFront() on an empty List" << endl;
		exit(1);
	}
	return(front->data);
}
	
/*
*  getBack
*  Returns the value at the back of L.
*  Pre: !isEmpty(L)
*/
int List::getBack(){
	if( isEmpty() ){
		cout << "List Error: calling getBack() on an empty List" << endl;
		exit(1);
	}
	return(back->data);
}

/*
*  getCurrent
*  Returns the value at the current node of L.
*  Pre: !isEmpty(L), !offEnd(L)
*/
int List::getCurrent(){
	if( isEmpty() ){
		cout << "List Error: calling getCurrent() on an empty List" << endl;
		exit(1);
	}
	if( offEnd() ){
		cout << "List Error: calling getCurrent() with NULL current" << endl;
		exit(1);
	}
	return(current->data);
}

/*
*  getIndex
*  Returns the index of the current node of L. Returns -1 if current is undefined.
*  Pre: !isEmpty(L)
*/
int List::getIndex(){
	if( isEmpty() ){
		cout << "List Error: calling getIndex() on an empty List" << endl;
		exit(1);
	}
	if( offEnd() ){
		return -1;
	}
	int i = -1;
	bool found = false;
	Node* test = front;
	while(found==false){
		if(test==current)
			found = true;
		if(test->next!=NULL)
			test = test->next;
		i++;
	}
	return i;
}

/*
*  offEnd
*  Returns True if current is undefined, otherwise returns false.
*/
int List::offEnd(){
	return(current==NULL);	
}

/*
*  getLength
*  Returns the length of L
*/
int List::getLength(){
	return(length);
}

/*
*  isEmpty
*  Returns True if L is empty, otherwise returns 0
*/
int List::isEmpty(){
	return(length==0);
}

// isIn
bool List::isIn(int v){
	if(isEmpty()){ return false; }
	int i = getIndex();
	moveTo(0);
	while(!offEnd()){
		if(getCurrent() == v){
			moveTo(i);
			return true;
		}
		moveNext();
	}
	moveTo(i);
	return false;
}
/*
*  insertBack
*  Places new data element at the end of L
*  Post: !isEmpty(L)
*/

void List::insertBack(int data)
{
	Node* N = new Node(data);
	if( isEmpty() ) { front = back = current = N; }
	else { back->next = N; N->prev = back; back = N;}
	length++;
}

/*
*  insertFront
*  Places new data element at the front of L
*  Post: !isEmpty(L)
*/
void List::insertFront(int data)
{
	Node* N = new Node(data);
	if( isEmpty() ) { front = back = current = N; }
	else { front->prev = N; N->next = front; front = N;}
	length++;
}

/*
*  insertBeforeCurrent
*  Places new data element before current element
*  Pre: !isEmpty(L), !offEnd(L)
*/
void List::insertBeforeCurrent(int data){
	Node* N = new Node(data);

	if(isEmpty()){
		cout << "List Error: calling insertBeforeCurrent() on empty List" << endl;
		exit(1);
	}
	if(offEnd()){
		cout << "List Error: calling insertBeforeCurrent() while current is NULL" << endl;
		exit(1);
	}
	if(getIndex()==0)
		insertFront(data);
	else{
		current->prev->next = N;
		N->prev = current->prev;
		current->prev = N;
		N->next = current;
		length++;
	}
}

/*
*  insertAfterCurrent
*  Places new data element after current element
*  Pre: !isEmpty(L), !offEnd(L)
*/
void List::insertAfterCurrent(int data){
	Node* N = new Node(data);

	if(isEmpty()){
		cout << "List Error: calling insertAfterCurrent() on empty List" << endl;
		exit(1);
	}
	if(offEnd()){
		cout << "List Error: calling insertAfterCurrent() while current is NULL" << endl;
		exit(1);
	}
	if(getIndex()==(length-1))
		insertBack(data);
	else{
		current->next->prev = N;
		N->next = current->next;
		current->next = N;
		N->prev = current;
		length++;
	}
}
	

/*
*  deleteFront
*  Deletes element at front of L
*  Pre: !isEmpty(L)
*/
void List::deleteFront(){
	Node* N = NULL;

	if( isEmpty() ){
		cout << "List Error: calling deleteFront() on an empty List" << endl;
		exit(1);
	}
	N = front;
	if( getLength()>1 ) { 
		if(current == front)
			moveNext();
		front = front->next; 
	}else { 
		front = back = current = NULL; 
	}
	length--;
	delete(N);
}

/*
*  deleteBack
*  Deletes element at back of L
*  Pre: !isEmpty(L)
*/
void List::deleteBack(){
	Node* N = NULL;

	if( isEmpty() ){
		cout << "List Error: calling deleteBack() on an empty List" << endl;
		exit(1);
	}

	N = back;
	if( getLength()>1 ) { 
		if(current == back);
			movePrev();
		back = back->prev;
	}else{
		front = NULL;
		back = NULL; 
		current = NULL;
	}
	length--;
	delete(N);
}

/*
*  deleteCurrent
*  Deletes the current element.
*  Pre: !isEmpty(L), !offEnd(L)
*/
void List::deleteCurrent(){
	Node* N = NULL;

	if( isEmpty() ){
		cout << "List Error: calling deleteCurrent() on an empty List" << endl;
		exit(1);
	}
	if( offEnd() ){
		cout << "List Error: calling deleteCurrent() with NULL current" << endl;
		exit(1);
	}
	if( current == back ){
		deleteBack();
	}else if( current == front ){
		deleteFront();
	}else{
		Node* tempPrev = current->prev;
		Node* tempNext = current->next;
		tempPrev->next = tempNext;
		tempNext->prev = tempPrev;
		N = current;
		current = NULL;
		delete(N);
		length--;
	}
}

/*
*  moveNext
*  Moves the current element one step forward in the list.
*  Pre: !isEmpty, !offEnd
*/
void List::moveNext(){
	if( isEmpty() ){
		cout << "List Error: calling moveNext() on an empty List" << endl;
		exit(1);
	}
	if( offEnd() ){
		cout << "List Error: calling moveNext() with NULL current" << endl;
		exit(1);
	}
	if(current==back){
		current = NULL;
	}
	else{
		current = current->next;
	}
}
	
/*
*  movePrev
*  Moves the current element one step back in the list.
*  Pre: !isEmpty, !offEnd
*/
void List::movePrev(){
	if( isEmpty() ){
		cout << "List Error: calling movePrev() on an empty List" << endl;
		exit(1);
	}
	if( offEnd() ){
		cout << "List Error: calling movePrev() with NULL current" << endl;
		exit(1);
	}
	if(current==front){
		current = NULL;
	}
	else{
		current = current->prev;
	}
}

/*
*  moveTo
*  Moves the current element to the indicated spot in the List.
*/
void List::moveTo(int i){
	if(isEmpty() || i < 0 || i > (getLength()-1))
		current = NULL;
	else{
		int n = i;
		current = front;
		while(n > getIndex())
			moveNext();
	}	
}

/*
*  makeEmpty
*  Empties out the list.
*  Post: isEmpty(L)
*/
void List::makeEmpty(){
	if(!isEmpty()){
		while(!isEmpty() ){
			deleteFront();
		}
		current = back = front = NULL;
		length = 0;
	}
}
	
/*
*  printList
*  Prints data elements in L on a single line to stdout.
*/

void List::printList(){

	for(Node* N = front; N != NULL; N = N->next){
		cout << N->data << " ";
	}
	cout << endl;
}

/*
*  equals
*  returns 1 if A is identical to B, 0 otherwise
*/
int List::equals(List* B){
	int flag = 1;
	Node* N = NULL;
	Node* M = NULL;
	if(B==NULL){
		cout << "List Error: calling equals() on NULL List*" << endl;
		exit(1);
	}
	N = front;
	M = B->front;
	if( length != B->length ) { return 0; }
	while( flag && N!=NULL){
		flag = (N->data==M->data);
		N = N->next;
		M = M->next;
	}
	delete N;
	delete M;
	return flag;
}

/*
*  copyList
*  Creates a new List identical to L.
*  Pre: !isEmpty(L)
*/
List* List::copyList(){
	if(isEmpty()){
		cout << "List Error: calling copyList() on empty List" << endl;
		exit(1);
	}
	List* P = new List();
	Node* N = front;
	while(N!=NULL){
		P->insertBack(N->data);
		N=N->next;
	}
	delete N;
	return P;
}
