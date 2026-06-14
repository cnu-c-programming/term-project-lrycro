#ifndef FILE_IO_H
#define FILE_IO_H

#include "student.h"

int load_csv(Student **head, const char *filename);

int save_csv(Student *head, const char *filename);

#endif // FILE_IO_H
