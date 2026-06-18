#include <stdio.h>
#include <string.h>
#include "command.h"
#include "student.h"
#include "file_io.h"

int g_unsaved_changes = 0;	// 변경 X: 0, 변경 O: 1
static int g_exit = 0;		// exit 경고 구분 flag

// 1. handler functions

ShellResult handle_save(char* args, Student** head) {
	(void)args;

	int cnt = save_csv(*head, g_csv_path);

	if (cnt == -1) {
		printf("Error: cannot save to file.\n");
		return SHELL_ERR_FILE_WRITE;
	}

	// TC32
	printf("Saved %d students to %s.\n", cnt, g_csv_path);

	g_unsaved_changes = 0;
	g_exit = 0;

	return SHELL_OK;
}

ShellResult handle_reload(char* args, Student** head) {
        (void)args;

	// to prevent memory leak
	free_students(*head);
	*head = NULL;

	int cnt = load_csv(head, g_csv_path);

	if (cnt == -1) {
		printf("Error: cannot open file.\n");
		return SHELL_ERR_FILE_OPEN;
	} else if (cnt == -2) {
		// TC05 CSV header error
		printf("Error: invalid header in CSV file.\n");
		return SHELL_EXIT;
	}

	// TC34
	printf("Reloaded %d students from %s.\n", cnt, g_csv_path);

	g_unsaved_changes = 0;
        g_exit = 0;

        return SHELL_OK;
}

ShellResult handle_add(char* args, Student** head) {
	// 잘못된 인자 (args가 NULL인 경우)
	if (args == NULL) {
		printf("Error: missing arguments.\n");
		return SHELL_ERR_MISSING_ARGUMENT;
	}

	int id, score;
	char name[64];

	// parse
	int parsed = sscanf(args, "%d %31s %d", &id, name, &score);

	// 잘못된 인자 (개수 차이, type error)
	if (parsed != 3) {
		printf("Error: invalid arguments.\n");
		return SHELL_ERR_INVALID_ARGUMENT;
	}

	// TC11 TC12
	if (id <= 0) {
		printf("Error: invalid ID.\n");
		return SHELL_ERR_INVALID_ARGUMENT;
	}

	// 점수 범위 (0 ~ 100)
	if (score < 0 || score > 100) {
		printf("Error: invalid score.\n");
		return SHELL_ERR_INVALID_SCORE;
	}

	// add_student 호출
	int result = add_student(head, id, name, score);

	// duplicate ID
	if (result == -1) {
		printf("Error: duplicate ID.\n");
		return SHELL_ERR_DUPLICATE_STUDENT;
	}

	// memory allocation error
	else if (result == -2) {
		printf("Error: memory allocation failed.\n");
		return SHELL_ERR_UNKNOWN_COMMAND;
	}

	// TC08, TC18+19 student added 반환
	printf("Student added.\n");

	g_unsaved_changes = 1;
        g_exit = 0;

	return SHELL_OK;
}

ShellResult handle_delete(char* args, Student** head) {
	if (args == NULL) {
		printf("Error: missing arguments.\n");
		return SHELL_ERR_MISSING_ARGUMENT;
	}

	int id;

	// id 추출
	if (sscanf(args, "%d", &id) != 1) {
		printf("Error: invalid arguments.\n");
		return SHELL_ERR_INVALID_ARGUMENT;
	}

	// 성공 0, 실패 -1
	int result = delete_student(head, id);

	// TC21 (소문자)
	if (result == -1) {
        	printf("Error: student not found.\n");
		return SHELL_ERR_STUDENT_NOT_FOUND;
	}

	// TC20
	printf("Student deleted.\n");

	g_unsaved_changes = 1;
        g_exit = 0;

	return SHELL_OK;
}

ShellResult handle_update(char* args, Student** head) {
	if (args == NULL) {
		printf("Error: missing arguments.\n");
		return SHELL_ERR_MISSING_ARGUMENT;
	}

	int id, new_score;

	if (sscanf(args, "%d %d", &id, &new_score) != 2) {
		printf("Error: invalid arguments.\n");
		return SHELL_ERR_INVALID_ARGUMENT;
	}

	if (new_score < 0 || new_score > 100) {
		printf("Error: invalid score.\n");
		return SHELL_ERR_INVALID_SCORE;
	}

	// 성공 0, 실패 -1
	int result = update_student(*head, id, new_score);

	if (result == -1) {
		// TC24
		printf("Error: student not found.\n");
		return SHELL_ERR_STUDENT_NOT_FOUND;
	}

	// TC23
	printf("Student updated.\n");

	g_unsaved_changes = 1;
        g_exit = 0;

	return SHELL_OK;
}

ShellResult handle_find(char* args, Student** head) {
        if (args == NULL) {
                printf("Error: missing arguments.\n");
                return SHELL_ERR_MISSING_ARGUMENT;
        }

        int id;

        // id 추출
        if (sscanf(args, "%d", &id) != 1) {
                printf("Error: invalid arguments.\n");
                return SHELL_ERR_INVALID_ARGUMENT;
        }

        Student* target = find_student(*head, id);

        // TC29 (소문자)
        if (target == NULL) {
                printf("Error: student not found.\n");
                return SHELL_ERR_STUDENT_NOT_FOUND;
        }

        printf("ID: %d\n", target->id);
        printf("Name: %s\n", target->name);
        printf("Score: %d\n", target->score);

	return SHELL_OK;
}

ShellResult handle_list(char* args, Student** head) {
	(void)args;
	list_students(*head);
	return SHELL_OK;
}

ShellResult handle_stats(char* args, Student** head) {
        (void)args;

	// TC37
	if (*head == NULL) {
		printf("No student data available\n");
		return SHELL_OK;
	}

	int cnt = 0;
	int sum = 0;
	int max = -1;
	int min = 101;

	Student* curr = * head;
	while (curr != NULL) {
		cnt++;
		sum += curr->score;
		if (curr->score > max) {
			max = curr->score;
		}
		if (curr->score < min) {
			min = curr->score;
		}
		curr = curr->next;
	}

	// TC36, TC38
	double avg = (double)sum / cnt;

	printf("Count: %d\n", cnt);
	printf("Average: %.1f\n", avg);
	printf("Max: %d\n", max);
	printf("Min: %d\n", min);

        return SHELL_OK;
}

ShellResult handle_help(char* args, Student** head);

ShellResult handle_clear(char* args, Student** head) {
	(void) args;
	(void) head;

	// ANSI escape sequence
	printf("\033[2J\033[H");

	return SHELL_OK;
}

ShellResult handle_exit(char* args, Student** head) {
        (void)args;
        (void)head;

	if (g_unsaved_changes == 1 && g_exit == 0) {
		printf("Warning: You have unsaved changes. Type 'exit' again to quit without saving.\n");
		g_exit = 1; // warning O
		return SHELL_OK;
	}

	printf("Goodbye.\n");

	return SHELL_EXIT;
}

ShellResult handle_sort(char* args, Student** head) {
	if (args == NULL) {
		printf("Error: missing arguments.\n");
		return SHELL_ERR_MISSING_ARGUMENT;
	}

	char key[32];
	if (sscanf(args, "%s", key) != 1) {
		printf("Error: invalid arguments.\n");
		return SHELL_ERR_INVALID_ARGUMENT;
	}

	// TC43
	if (strcmp(key, "name") != 0 && strcmp(key, "score") != 0) {
		printf("Error: invalid sort key.\n");
		return SHELL_ERR_INVALID_ARGUMENT;
	}

	if (*head == NULL || (*head)->next == NULL) {
		// TC40 sort name messages
		if (strcmp(key, "name") == 0) printf("sorted by name.\n");
		if (strcmp(key, "score") == 0) printf("sorted by score.\n");
		return SHELL_OK;
	}

	int cnt = 0;
	Student* tmp = *head;
	while (tmp != NULL) {
	    cnt++;
    	    tmp = tmp->next;
	}

	for (int i = 0; i < cnt - 1; i++) {
	    Student** curr = head;
    	    for (int j = 0; j < cnt - i - 1; j++) {
	        Student* p1 = *curr;
		Student* p2 = p1->next;
		int needs_swap = 0;

		if (strcmp(key, "name") == 0) {
		    if (strcmp(p1->name, p2->name) > 0) needs_swap = 1;
		} else if (strcmp(key, "score") == 0) {
		    if (p1->score > p2->score) needs_swap = 1;
		}

		if (needs_swap) {
		    p1->next = p2->next;
		    p2->next = p1;
		    *curr = p2;
		}
		curr = &((*curr)->next);
	    }
	}

	// TC40
	if (strcmp(key, "name") == 0) {
		printf("sorted by name.\n");
	} else {
		printf("sorted by score.\n");
	}

	g_unsaved_changes = 1;
        g_exit = 0;

	return SHELL_OK;
}

// 2. command table

#ifdef ADMIN_MODE
Command commands[] = {
    {"save",   handle_save,   "save",                       "Save students to CSV"},
    {"reload", handle_reload, "reload",                     "Reload students from CSV"},
    {"add",    handle_add,    "add <id> <name> <score>",    "Add a student"},
    {"delete", handle_delete, "delete <id>",                "Delete a student"},
    {"update", handle_update, "update <id> <score>",        "Update student score"},
    {"find",   handle_find,   "find <id>",                  "Find student"},
    {"list",   handle_list,   "list",                       "List students"},
    {"stats",  handle_stats,  "stats",                      "Show statistics"},
    {"help",   handle_help,   "help",                       "Show help"},
    {"clear",  handle_clear,  "clear",                      "Clear screen"},
    {"exit",   handle_exit,   "exit",                       "Exit shell"},
    {"sort",   handle_sort,   "sort <name|score>",	    "Sort students"}
};
#endif

#ifdef CLIENT_MODE
Command commands[] = {
    {"reload", handle_reload, "reload",      	    "Reload students from CSV"},
    {"find",   handle_find,   "find <id>",    	    "Find student"},
    {"list",   handle_list,   "list",         	    "List students"},
    {"stats",  handle_stats,  "stats",         	    "Show statistics"},
    {"help",   handle_help,   "help",         	    "Show help"},
    {"clear",  handle_clear,  "clear",        	    "Clear screen"},
    {"exit",   handle_exit,   "exit",         	    "Exit shell"},
    {"sort",   handle_sort,   "sort <name|score>",  "Sort students"}
};
#endif

int cnt_commands = sizeof(commands) / sizeof(Command);

// help handler function
ShellResult handle_help(char* args, Student** head) {
	(void)args;
	(void)head;
	printf("Commands:\n");
	for (int i = 0; i < cnt_commands; i++) {
		printf("%-30s %s\n", commands[i].usage, commands[i].description);
	}
	return SHELL_OK;
}

// 3. main parsing function

int proc_cmd(Student **head, const char *cmd_line) {
	char buf[256];

	strncpy(buf, cmd_line, sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';

	int len = strlen(buf);
	while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) {
		buf[len - 1] = '\0';
		len--;
	}

	if (len == 0) return 1;

	char cmd[32] = {0};
	char *args = NULL;

	for (int i = 0; buf[i] != '\0'; i++) {
		if (buf[i] == ' ') {
			buf[i] = '\0';
			cmd[i] = '\0';
			args = &buf[i + 1];
			break;
		}
		cmd[i] = buf[i];
	}

	// 4. 테이블에서 명령어 찾기 및 실행
	for (int i = 0; i < cnt_commands; i++) {
		if (strcmp(cmd, commands[i].name) == 0) {
			// 명령어 찾으면 핸들러 함수 실행
			ShellResult result = commands[i].handler(args, head);

			if (result == SHELL_EXIT) {
				return 0;
			}
			if (result != SHELL_OK) {
				return -1; // error 발생 시 -1 return
			}
			return 1;
		}
	}

	printf("Error: unknown command or permission denied.\n");

	return 1; // shell 계속 실행
}
