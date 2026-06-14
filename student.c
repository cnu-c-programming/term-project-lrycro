#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

// 1. add student: 성공(0), 중복(-1), 메모리 부족(-2)
int add_student(Student** head, int id, const char* name, int score) {
	if (find_student(*head, id) != NULL) {
		return -1;
	}

	Student* new_student = (Student*)malloc(sizeof(Student));
	
	if (new_student == NULL) {
		return -2;
	}
	
	new_student->id = id;
	strncpy(new_student->name, name, sizeof(new_student->name) - 1);
	new_student->name[sizeof(new_student->name) - 1] = '\0';
	new_student->score = score;
	new_student->next = NULL;

	if (*head == NULL) {
		*head = new_student;
	} else {
		Student* curr = *head;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = new_student;
	}
	
	return 0;
}

// 2. delete student: 성공(0), 데이터 없음(-1) 
int delete_student(Student** head, int id) {
	if (*head == NULL) {
		return -1;
	}

	if ((*head)->id == id) {
		Student* tmp = *head;
		*head = (*head)->next;
		free(tmp);
		return 0;
	}

	Student* curr = *head;
	
	while (curr->next != NULL) {
		if (curr->next->id == id) {
			Student* tmp = curr->next;
			curr->next = tmp->next;
			free(tmp);
			return 0;
		}
		curr = curr->next;
	}
	return -1;
}

// 3. find student
Student* find_student(Student* head, int id) {
	Student* curr = head;

	while (curr != NULL) {
		if (curr->id == id) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

// 4. update student's score: 성공(0), 실패(-1)
int update_student(Student* head, int id, int new_score) {
	Student* target = find_student(head, id);
	if (target == NULL) {
		return -1;
	}
	target->score = new_score;
	return 0;
}

// 5. print list of students
void list_students(Student* head) {
	if (head == NULL) {
		// TC07
		printf("No students found.\n");
		return;
	}

	printf("ID\tName\t\tScore\n");

	Student* curr = head;
	
	while (curr != NULL) {
		printf("%d\t%-15s\t%d\n", curr->id, curr->name, curr->score);
		curr = curr->next;
	}
}

// 6. memory free
void free_students(Student* head) {
	Student* curr = head;
	Student* next_node;

	while (curr != NULL) {
		next_node = curr->next;
		free(curr);
		curr = next_node;
	}
}
