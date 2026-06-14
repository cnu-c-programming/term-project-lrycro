#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "student.h"

int load_csv(Student **head, const char *filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
	return -1;  // file open failure
    }

    int cnt = 0;
    char line[256];
    
    if (fgets(line, sizeof(line), fp) != NULL) {
	int len = strlen(line);
	while (len > 0 && line[len - 1] == '\n') {
	    line[len - 1] = '\0';
	    len--;
	}

	if (strcmp(line, "id,name,score") != 0) {
	    fclose(fp);
	    return -2;  // wrong csv header (TC05)
	}
    } else {
	fclose(fp);
	return 0;  // empty file
    }

    // header check
    while (fgets(line, sizeof(line), fp) != NULL) {
	int len = strlen(line);
	
        while (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }

	if (len == 0) continue;

	char *id_ptr = line;
	char *name_ptr = NULL;
	char *score_ptr = NULL;

	// 첫 쉼표 -> \0, name_ptr
	for (int i = 0; line[i] != '\0'; i++) {
	    if (line[i] == ',') {
		line[i] = '\0';
		name_ptr = &line[i + 1];
		break;
	    }
	}

	// 두 번째 쉼표 -> \0, score_ptr
	if (name_ptr != NULL) {
	    for (int i = 0; name_ptr[i] != '\0'; i++) {
		if (name_ptr[i] == ',') {
		    name_ptr[i] = '\0';
		    score_ptr = &name_ptr[i + 1];
		    break;
		}
	    }
	}

	// 3개 값 null check 후 not null이면 linked list에 add
	if (id_ptr != NULL && name_ptr != NULL && score_ptr != NULL) {
	    int id = atoi(id_ptr);
	    int score = atoi(score_ptr);

	    if (add_student(head, id, name_ptr, score) == 0) {
		cnt++;
	    }
	}
    }

    fclose(fp);
    return cnt;

}

int save_csv(Student *head, const char *filename) {
    
    return 0;
}
