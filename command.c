#include <stdio.h>
#include <string.h>
#include "command.h"
#include "student.h"

// 1. handler functions

ShellResult handle_save(char* args, Student** head) {
	(void)args;
	(void)head;
	printf("Save function 구현 예정\n");
	return SHELL_OK;
}

ShellResult handle_reload(char* args, Student** head) {
        (void)args;
        (void)head;
        printf("Reload function 구현 예정\n");
        return SHELL_OK;
}

ShellResult handle_add(char* args, Student** head) {
        (void)args;
        (void)head;
        printf("Add function 구현 예정\n");
        return SHELL_OK;
}

ShellResult handle_delete(char* args, Student** head) {
        (void)args;
        (void)head;
        printf("Delete function 구현 예정\n");
        return SHELL_OK;
}

ShellResult handle_update(char* args, Student** head) {
        (void)args;
        (void)head;
        printf("Update function 구현 예정\n");
        return SHELL_OK;
}

ShellResult handle_find(char* args, Student** head) {
        (void)args;
        (void)head;
        printf("Find function 구현 예정\n");
        return SHELL_OK;
}

ShellResult handle_list(char* args, Student** head) {
	(void)args;
	list_students(*head);
	return SHELL_OK;
}

ShellResult handle_stats(char* args, Student** head) {
        (void)args;
        (void)head;
        printf("Stats function 구현 예정\n");
        return SHELL_OK;
}

ShellResult handle_help(char* args, Student** head);

ShellResult handle_clear(char* args, Student** head) {
        (void)args;
        (void)head;
        printf("Clear function 구현 예정\n");
        return SHELL_OK;
}

ShellResult handle_exit(char* args, Student** head) {
        (void)args;
        (void)head;
        return SHELL_EXIT;
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
    {"exit",   handle_exit,   "exit",                       "Exit shell"}
};
#endif

#ifdef CLIENT_MODE
Command commands[] = {
    {"reload", handle_reload, "reload",      "Reload students from CSV"},
    {"find",   handle_find,   "find <id>",    "Find student"},
    {"list",   handle_list,   "list",         "List students"},
    {"stats",  handle_stats,  "stats",        "Show statistics"},
    {"help",   handle_help,   "help",         "Show help"},
    {"clear",  handle_clear,  "clear",        "Clear screen"},
    {"exit",   handle_exit,   "exit",         "Exit shell"}
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
	while (len > 0 && buf[len - 1] == '\n') {
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
			return 1;
		}
	}

	printf("Error: unknown command\n");

	return 1; // shell 계속 실행
}
