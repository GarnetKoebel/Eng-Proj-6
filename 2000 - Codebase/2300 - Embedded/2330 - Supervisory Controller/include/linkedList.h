/*
 *	All Rights Reserved
 *
 *	Copyright 2020(c) Garnet Koebel
 * 
 * 
 *	Library Name: 			Koebel Bi-Directional Linked List Library
 *	Library Author: 		Garnet Koebel
 *	Library Version: 		V0.3
 *	Library Created:		2020-04-08
 *	Last Library Update:	2020-11-12
 *	Library Description:	This library contains basic linked list
 *							functionality. This library supports linked lists with custom datatypes
 *
 *	UPDATE NOTES
 *
 *	Updates For V0.1
 *	-Implemented removeElement function
 *	-Implemented copyList function
 *	-Implemented replaceElement function
 *	-Wrote function decriptions
 *	-Added typedef for linkedList
 *
 *	Updates For V0.2
 *	-Tweaked error messages
 *	-changed printAllElements to type int
 *	-Improved error handling for findByIndex (function can now handle empty list)
 *
 *	Updates For V0.3
 *	-Switched listElement datatype to only contain a custom struct. This will simplify adding additional data to linked list elements
 *	-Added support for bi-directional linked lists (doubley linked lists)
 *  -Replaced all occurences of struct listElement with linkedList to improve readability
 * 
 */
#ifndef DUO_LINKED_LIST_LIB_H
#define DUO_LINKED_LIST_LIB_H
 //Required Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Custom Datatypes
typedef struct listElement linkedList;
struct listElement {
	void* dataStruct;
	int dataInt;			  //useful as a unique identifier (e.g. use if mapping list to an array)
	linkedList* next; //pointer to the next element
	linkedList* prev; //pointer to the previous element
};
linkedList* createList(); //Create List
linkedList* findByIndex(linkedList* targetList, int targetIndex); //Search elements
linkedList* findFirstByID(linkedList* targetList, int targetID);
linkedList* copyList(linkedList* sourceList);
int deleteList(linkedList* targetList); //Delete List
int printAllElements(linkedList* targetList); //Iterate the list
int addElement(int intID, void* data, linkedList** targetList, int targetIndex, int size); //Add elements
int removeElement(linkedList** targetList, int targetIndex);
int replaceElement(int intID, void* dataStruct, linkedList** targetList, int targetIndex);
int getElementPosById(linkedList* targetList, int targetID);
#endif