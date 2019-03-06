#include "LinkedList.h"

namespace LinkedList {
	ListNode* LinkedList::NewNode(int* _data) {
		return new ListNode(_data);
	}

	void LinkedList::PrintList(ListNode* head) {
		ListNode* cur = head;
		while (cur != NULL) {
			std::cout << cur->data << std::endl;
			cur = cur->next;
		}
	}

	void LinkedList::DeleteList(ListNode* head) {
		ListNode* cur = head;
		while (cur != NULL) {
			ListNode* temp = cur;
			cur = cur->next;
			delete temp;
		}
	}

	ListNode* LinkedList::InsertBeforeHead(ListNode* head, ListNode* newHead) {
		newHead->next = head;
		return newHead;
	}

	ListNode* LinkedList::GetNodeAtIndex(ListNode* head, int index) {
		ListNode* cur = head;
		for (int i = 0; i < index; i++) {
			if (cur == NULL)
				return NULL;

			cur = cur->next;
		}

		return cur;
	}

	ListNode* LinkedList::GetNodeByValue(ListNode* head, int* value) {
		ListNode* cur = head;
		while (cur != NULL) {
			if (cur->data == value)
				return cur;
			else
				cur = cur->next;
		}

		return NULL;
	}

	void LinkedList::InsertNodeAfter(ListNode* insertAfter, ListNode* newNode) {
		newNode->next = insertAfter->next;
		insertAfter->next = newNode;
	}

	void LinkedList::DeleteNodeAfter(ListNode* deleteAfter) {
		if (deleteAfter->next != NULL && deleteAfter != NULL) {
			ListNode* temp = deleteAfter->next;
			deleteAfter->next = deleteAfter->next->next;
			delete temp;
		}
	}

	void AddNodeAtEnd(ListNode* head, ListNode* newNode) {
		ListNode* cur = head;
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = newNode;
	}
}