#ifndef STUDENT_H
#define STUDENT_H

typedef struct Student {
	int id;
	char name[32];
	int score;
	struct Student* next;
} Student;

Student* add_student(Student* head, int id, const char* name, int score);
Student* delete_student(Student* head, int id);
Student* find_student(Student* head, int id);

void list_students(Student* head);
void free_students(Student* head);

#endif // STUDENT_H
