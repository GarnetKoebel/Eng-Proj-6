/*
 *	All Rights Reserved
 *
 *	Copyright 2020(c) Garnet Koebel
 * 
 *	Library Name: 			Bi-Directional Linked List
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
 */
#include "../include/linkedList.h"
 //Create List
linkedList* createList() {
	/*
	 *	Function Name: 		createList
	 *	Function Type: 		listElement*
	 *	Function Inputs: 	none
	 *	Function Outputs: 	none
	 *	Error Handling:		No
	 *	Function Description:	This function creates an variable of type listElement
	 *							which acts as the head of a linked list.
	 *
	*/
	linkedList* emptyList = NULL;
	return emptyList;
}
linkedList* findFirstByID(linkedList* targetList, int targetID) {
	/*
	 *	Function Name: 		*findFirstByID
	 *	Function Type: 		listElement
	 *	Function Inputs: 	listElement *targetList, int targetID
	 *	Function Outputs: 	listElement currentElement
	 *	Error Handling:		Yes
	 *	Function Description:	This function allows for finding and elements data by ID.
	 *							Function can handle no ID found errors. Returns NULL on
	 *							error. Returns pointer to currentElement on successful run.
	 *
	*/
        if (targetList == NULL) {
         return NULL;
        }
	linkedList* currentElement = targetList;
	while (currentElement->next != NULL) {
		if (currentElement->dataInt == targetID) {
			//printf("Found Element ID %d: %s with ID(%d)...\n", 
			//currentElement->dataInt, currentElement->dataName, targetID);
			return currentElement; //return pointer and exit function
		}
		currentElement = currentElement->next;
	}
	if (currentElement->dataInt == targetID) {
		//printf("Found Element ID %d: %s with ID(%d)...\n", 
		//currentElement->dataInt, currentElement->dataName, targetID);
		return currentElement; //return pointer and exit function
	}
	else { //print error message return null
	//	printf("Element with ID(%d) not found.\n", targetID);
		return NULL;
	}
}
//Search elements
linkedList* findByIndex(linkedList* targetList, int targetIndex) {
	/*
	 *	Function Name: 		*findByIndex
	 *	Function Type: 		listElement
	 *	Function Inputs: 	listElement *targetList, int targetIndex
	 *	Function Outputs: 	listElement currentElement
	 *	Error Handling:		Yes
	 *	Function Description:	This function allows for finding data in a list element
	 *							by index. Function can handle out of bounds errors.
	 *							Returns NULL on error. Returns pointer to currentElement
	 *							on successful run.
	 *
	*/
	linkedList* currentElement = targetList;
	if (targetList == NULL) { //if list empty don't do anything
		printf("ERR_LIST_EMPTY");
		return NULL;
	}
	for (int i = 0; i < targetIndex; i++) {
		if (currentElement->next == NULL) {
			printf("ERROR: Index out of bound, element not found at index(%d)! \n", targetIndex);
			return NULL; //indicating error;
		}
		currentElement = currentElement->next;
	}
	//printf("Found Element ID %d: %s at index(%d)...\n", 
		//	currentElement->dataInt, currentElement->dataName, targetIndex);
	return currentElement;
}
linkedList* copyList(linkedList* sourceList, int size) {
	/*
	 *	Function Name: 		*copyList
	 *	Function Type: 		listElement
	 *	Function Inputs: 	listElement *sourceList
	 *	Function Outputs: 	listElement targetList
	 *	Error Handling:		No
	 *	Function Description:	This function allows for copying a source list to
	 *							a target list. This function will copy the source
	 *							element by element until it reaches the tail of
	 *							the source list. Function returns a pointer to
	 *							the head of the copied list on completion.
	 *
	*/
	linkedList* targetList = NULL; //Create target list pointer
	linkedList* currentSourceElement = sourceList; //read head for sourceList
	int i = 0; //counter variable for function print statements
	if (sourceList == NULL) { //list empty error and exit
		printf("ERR_LIST_EMPTY");
		return NULL;
	}
	while (currentSourceElement->next != NULL) { //THIS MAY BE WRONG
		//Add copied element to tail of targetList
	//	printf("Copying element index: %d\n", i); //print message
		addElement(currentSourceElement->dataInt, currentSourceElement->dataStruct, &targetList, -1, size); //call add element on currentSourceElement
		currentSourceElement = currentSourceElement->next; //increment read head
		i++; //increment counter
	}
	return targetList; //return pointer to head of list
}
//Delete List
int deleteList(linkedList* targetList) {
	/*
	 *	Function Name: 		deleteList
	 *	Function Type: 		int
	 *	Function Inputs: 	listElement *targetList
	 *	Function Outputs: 	return state
	 *	Error Handling:		No
	 *	Function Description:	This function allows for deleting linked lists. Returns 0 on successful run.
	 *
	*/
	linkedList* currentElement, * freeElement;
	currentElement = targetList;
	while (currentElement->next != NULL) {
		freeElement = currentElement;
		currentElement = currentElement->next;
		//printf("Releasing Element ID %d: %s...\n", freeElement->dataInt, freeElement->dataName);
		free(freeElement);
	}
	//	printf("Releasing Element ID %d: %s...\nReleasing Completed\n", currentElement->dataInt, currentElement->dataName);
	free(currentElement); //free the last element
	return 0;
}
//Iterate the list
int printAllElements(linkedList* targetList) {
	/*
	 *	Function Name: 		printAllElements
	 *	Function Type: 		int
	 *	Function Inputs: 	listElement* targetList
	 *	Function Outputs: 	none
	 *	Error Handling:		No
	 *	Function Description:	This function prints the contents of a linked list to the screen.
	 *
	*/
	linkedList* currentList = targetList; //I do not want to modify the incoming list address
	printf("\n=== LIST CONTENTS ===\n\n");
	while (currentList->next != NULL) {
		printf("Element ID %d: %s\n", currentList->dataInt, currentList->dataStruct);
		currentList = currentList->next;
	}
	printf("Element ID %d: %s\n", currentList->dataInt, currentList->dataStruct);
	printf("\n===== LIST END =====\n\n");
	return 0;
}
//Add elements
int addElement(int intId, void* data, linkedList** targetList, int targetIndex, int size) {
	/*
	 *	Function Name: 		addElement
	 *	Function Type: 		int
	 *	Function Inputs: 	int intID, char stringData[], listElement **targetList, int targetIndex
	 *	Function Outputs: 	Return State
	 *	Error Handling:		Yes
	 *	Function Description:	This function allows for adding elements to a linked list.
	 *							Function can handle out of bounds errors. Returns 1 on error.
	 *							Returns 0 on successful run.
	 *
	*/
	
	linkedList* currentElement = *targetList;
	linkedList* tempElement = (linkedList*)malloc(sizeof(linkedList)+size);
	tempElement->dataStruct = data;
	tempElement->dataInt = intId;
	//strcpy(tempElement->dataName, stringData);
	if (*targetList == NULL) {  //if the list is empty, just add the element directly regardless of the targetIndex
		tempElement->next = NULL;
		tempElement->prev = NULL;
		*targetList = tempElement;
	}
	else if (targetIndex == 0) {  //if adding to the head element, don't even iterate
		tempElement->next = *targetList;
		tempElement->prev = NULL;
		*targetList = tempElement;
	}
	else if (targetIndex == -1) { //if adding to tail element, iterate through the entire list
		linkedList* lastElement = (linkedList*)malloc(sizeof(linkedList));
		while (currentElement->next != NULL) {
			lastElement->next = currentElement; //save address
			currentElement = currentElement->next; //increment to next element
		}
		tempElement->next = NULL;
		currentElement->next = tempElement;
		currentElement->prev = lastElement->next;
	}
	else { //Otherwise, heed the index-out-of-bound issue, and add to the right place
		for (int i = 0; i < targetIndex - 1; i++) {
			if (currentElement->next == NULL) {
				printf("ERROR: Index out of bound, element not added! \n");
				free(tempElement);
				return 1; //indicating error;
			}
			currentElement = currentElement->next;
		}
		tempElement->next = currentElement->next;
		currentElement->next = tempElement;
	}
	return 0;
}
int removeElement(linkedList** targetList, int targetIndex) {
	/*
	 *	Function Name: 		removeElement
	 *	Function Type: 		int
	 *	Function Inputs: 	listElement **targetList, int targetIndex
	 *	Function Outputs: 	Return State
	 *	Error Handling:		Yes
	 *	Function Description:	This function allows for removing elements at any index.
	 *							Function uses two read heads with and offset of one between
	 *							them to accomplish its task. Function can handle out of bounds
	 *							errors. Returns 1 on error. Returns 0 on successful run.
	 *
	*/
	/*
	 *	Function Theory of Operation
	 *
	 *	1. Get pointer to head of targetList
	 *	2. Get pointer of next element
	 *	3. Continue to get next pointer until targetIndex
	 *	4. Set pointer at targetIndex-1 to equal the pointer at targetIndex
	 *	5. Free pointer at targetIndex
	*/
	linkedList* currentElement = *targetList; //Get pointer to head of list
	if (*targetList == NULL) { //if list empty don't do anything
		printf("ERR_LIST_EMPTY");
		return 1;
	}
	else if (targetIndex == 0) { //if removing head skip iteration
		linkedList* nextElement = (linkedList*)malloc(sizeof(linkedList));; //create second pointer to store address of next element
		nextElement = currentElement->next; //get pointer of second element
		nextElement->prev = NULL;			//Set prev pointer of new head to null
		//printf("Removing Element ID %d: %s...\n", currentElement->dataInt, currentElement->dataName);
		free(currentElement);
		*targetList = nextElement; //set second elements pointer to *targetList
	}
	else if (targetIndex == -1) { //if removing from tail iterate through whole list
		linkedList* prevElement = (linkedList*)malloc(sizeof(linkedList));; //create second pointer to store address of previous element
		while (currentElement->next != NULL) { //iterate till tail
			prevElement = currentElement; //set previous element
			currentElement = currentElement->next; //set read head to next value
		}
		prevElement->next = NULL; //set prevElement as new tail
		free(currentElement); //free element at tail
	}
	else { //iterate till at targetIndex
		linkedList* nextElement = (linkedList*)malloc(sizeof(linkedList));; //create second pointer to store address of next element
		for (int i = 0; i < targetIndex - 1; i++) { //iterate through list until at targetIndex
			if (currentElement->next == NULL) { //check if about to go out of bounds
				printf("ERR_INDEX_OUT_OF_BOUND");
				free(currentElement);
				return 1;
			}
			currentElement = currentElement->next; //set read head to next value
			nextElement = currentElement->next; //set second read head to next value+1
		}
		//nextElement is the element to remove.
		//set currentElement.next to nextElement.next
		currentElement->next->prev = currentElement; //Set new next element to point backwards to currentElement
		currentElement->next = nextElement->next;	 //Set current element to point to new next element.
		free(nextElement); //free nextElement
	}
	return 0;
}
int replaceElement(int intID, void* dataStruct, linkedList** targetList, int targetIndex) {
	/*
	 *	Function Name: 		replaceElement
	 *	Function Type: 		int
	 *	Function Inputs: 	int intID, void* dataStruct, listElement **targetList, int targetIndex
	 *	Function Outputs: 	Return State
	 *	Error Handling:		Yes
	 *	Function Description:	This function allows for replacing an element with new element data.
	 *							This function can handles out of bounds errors. Returns 1 on error.
	 *							Returns 0 on successful run.
	 *
	*/
	/*
	 *	Function Theory of Operation
	 *
	 *	1. Increment read head to targetIndex
	 *	2. Overwrite data at targetIndex
	*/
	linkedList* currentElement = *targetList; //Get pointer to head of list
	//Create and allocate memory to a temp element to store the data to replace
	linkedList* tempElement = (linkedList*)malloc(sizeof(linkedList));
	if (*targetList == NULL) { //list empty error and exit
		printf("ERR_LIST_EMPTY");
		return 1;
	}
	else if (targetIndex == 0) { //replace at head skip read iteration
	 //replace data at head
	 //printf("Replacing ID %d with ID %d\n", currentElement->dataInt, intID);
		currentElement->dataInt = intID;
		currentElement->dataStruct = dataStruct;
	}
	else if (targetIndex == -1) { //replace at tail iterate till end of list
		while (currentElement->next != NULL) {
			currentElement = currentElement->next; //move read head
		}
		//replace data at tail
		//printf("Replacing ID %d with ID %d\n", currentElement->dataInt, intID);
		currentElement->dataInt = intID;
		currentElement->dataStruct = dataStruct;
	}
	else {
		for (int i = 0; i < targetIndex; i++) { //increment till targetIndex
			if (currentElement->next == NULL) { //check if about to go out of bounds
				printf("ERR_INDEX_OUT_OF_BOUND (Index %d)\n", targetIndex);
				//free(currentElement);
				return 1;
			}
			currentElement = currentElement->next; //move read head to next element
		}
		//replace data at targetIndex
		//printf("Replacing ID %d with ID %d\n", currentElement->dataInt, intID);
		currentElement->dataInt = intID;
		currentElement->dataStruct = dataStruct;
	}
	return 0;
}

int getElementPosById(linkedList* targetList, int targetID) {
	/*
	 *	Function Name: 		
	 *	Function Type: 		listElement
	 *	Function Inputs: 	listElement *targetList, int targetID
	 *	Function Outputs: 	
	 *	Error Handling:		No
	 *	Function Description:
	 *
	*/
	int index = 0; // track index position in list
	linkedList* currentElement = targetList;
	while (currentElement->next != NULL) {
		if (currentElement->dataInt == targetID) {
			//removeElement(targetList, index);
			return index;
		}
		index++;
		currentElement = currentElement->next;
	}
	if (currentElement->dataInt == targetID) {
		//removeElement(targetList, index);
		return index;
	}
	else { //return error
		return -1; // NOTE: this will cause the tail to be removed if not properly handled (if this was passed to removeElement())
	}
}

int removeAllById(linkedList* targetList, int targetID) {
	int index = 0; // track index position in list
	linkedList* currentElement = targetList;
	if (targetList == NULL || currentElement == NULL || currentElement == 0x0) {
            return -1; // return error code
        }
        while (currentElement->next != NULL) {
		if (currentElement->dataInt == targetID) {
			removeElement(&targetList, index);
		}
		index++;
		currentElement = currentElement->next;
	}
	if (currentElement->dataInt == targetID) {
		removeElement(&targetList, index);
	}
	else { //return error
		return 0;
	}
	
}
