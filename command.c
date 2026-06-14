#include <stdio.h>
#include <string.h>

#include "command.h"
#include "student.h"

int proc_cmd(Student **head, const char *cmd_line) {
	char buf[256];

	strncpy(buf, cmd_line, sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';

	char cmd[32] = {0};
	char arg1[32] = {0};
	char arg2[32] = {0};
	char arg3[32] = {0};

	int num_args = sscanf(buf, "%s %s %s %s", cmd, arg1, arg2, arg3);

	// no input
	if (num_args <= 0) {
		return 1;
	}

	// exit
	if (strcmp(cmd, "exit") == 0) {
		return 0;
	}
	// h3lp
	else if (strcmp(cmd, "help") == 0) {
		printf("Commands: list, find, add, delete, update, help, exit\n");
	}
	// list
	else if (strcmp(cmd, "list") == 0) {
		list_students(*head);
	}
	// TODO: find, add, delete 추가해야 함 일단 테스트 먼저 해보자
	else {
		printf("Error: unkown command.\n");
	}

	return 1; // shell 계속 실행
}
