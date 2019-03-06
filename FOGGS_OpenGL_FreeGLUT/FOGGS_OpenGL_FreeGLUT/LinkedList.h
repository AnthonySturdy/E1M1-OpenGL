#pragma once

#include "Structures.h"

namespace LinkedList {
	ListNode* NewNode(int* _data);
	void PrintList(ListNode* head);
	void DeleteList(ListNode* head);
	ListNode* InsertBeforeHead(ListNode* head, ListNode* newHead);
	ListNode* GetNodeAtIndex(ListNode* head, int index);
	ListNode* GetNodeByValue(ListNode* head, int* value);
	void InsertNodeAfter(ListNode* insertAfter, ListNode* newNode);
	void DeleteNodeAfter(ListNode* deleteAfter);
	void AddNodeAtEnd(ListNode* head, ListNode* newNode);
}