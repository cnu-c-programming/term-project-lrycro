/*
 * main.c  –  Mini Student Shell
 *
 * TODO: Implement admin_shell and client_shell.
 *
 * Build:
 *   make admin   →  admin_shell  (compiled with -DADMIN_MODE)
 *   make client  →  client_shell (compiled with -DCLIENT_MODE)
 *
 * Usage:
 *   ./admin_shell [students.csv]
 *   ./admin_shell -f commands.txt [students.csv]
 *   ./client_shell [students.csv]
 *   ./client_shell -f commands.txt [students.csv]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Add your own header includes here */
#include "student.h"
#include "file_io.h"
#include "command.h"

/* ---------------------------------------------------------------
 * TODO: Implement the interactive shell loop.
 *   - Print a prompt and read a line from stdin.
 *   - Parse the line into a command and arguments.
 *   - Dispatch to the appropriate handler function.
 *   - Loop until the user types "exit" or EOF.
 * --------------------------------------------------------------- */
void run_shell(const char *csv_path) {
    /* TODO */
//    (void)csv_path;
    /* temporary test log */
//    printf("Linked List test\n\n");
//
//    Student* head = NULL;
//
//    printf("1. Add 3 Students\n");
//    head = add_student(head, 1, "Alice", 90);
//    head = add_student(head, 2, "Bob", 85);
//    head = add_student(head, 3, "Charlie", 95);
//
//    printf("\n2. Print list of students\n");
//    list_students(head);
//
//    printf("\n3. Update Student's Score\n");
//    head = update_student(head, 2, 100);
//    list_students(head);
//
//    printf("\n4. Delete Student Data\n");
//    head = delete_student(head, 1);
//    list_students(head);
//
//    printf("\n5. TC07 No students found. Test\n");
//    head = delete_student(head, 2);
//    head = delete_student(head, 3);
//    list_students(head);
//
//    printf("\n6. free students Test\n");
//    free_students(head);
//    printf("\n7. test ends\n");

	Student* head = NULL;

	if (csv_path != NULL) {
		int loaded_count = load_csv(&head, csv_path);

		if (loaded_count == -1) {
			// if there's no file -> start with empty list, do nothing
		} else if (loaded_count == -2) {
			// wrong csv header (TC05)
			fprintf(stderr, "Error: invalid header in CSV file.\n");
			return;
		} else {
			// success
			printf("Loaded %d students from %s. \n", loaded_count, csv_path);
		}
	}

	/* 명령어 처리 */
	char line[256];

	while (1) {
#ifdef ADMIN_MODE
		printf("admin> ");

#elif defined(CLIENT_MODE)
		printf("client> ");

#endif

		if (fgets(line, sizeof(line), stdin) == NULL) {
			break;  // EOF(ctrl+d) 입력 시 종료
		}

		if (proc_cmd(&head, line) == 0) {
			break;  // 0 -> exit
		}
	}

	free_students(head);
}

/* ---------------------------------------------------------------
 * TODO: Implement batch mode – read commands from a file.
 *   - Open cmd_file for reading.
 *   - Execute each line as a command (same logic as run_shell).
 *   - Close the file when done.
 * --------------------------------------------------------------- */
void run_command_file(const char *cmd_file, const char *csv_path) {
    /* TODO */
    /* temporary log */
    printf("Command File Mode: cmd: %s, csv: %s\n", cmd_file, csv_path);
}

int main(int argc, char *argv[]) {
    const char *csv_path  = NULL; /* "students.csv"; */ /* default CSV file */
    const char *cmd_file  = NULL;           /* -f <file> argument */

    /* TODO: Parse command-line arguments.
     *   Supported flags:
     *     -f <file>   run commands from <file> instead of stdin
     *   Remaining positional argument (if any): path to students CSV.
     *
     *   Example parsing skeleton:
     *
     *   for (int i = 1; i < argc; i++) {
     *       if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
     *           cmd_file = argv[++i];
     *       } else {
     *           csv_path = argv[i];
     *       }
     *   }
     */
    for (int i = 1; i < argc; i++) {
	if (strcmp(argv[i], "-f") == 0) {
	    if (i + 1 < argc) {
		cmd_file = argv[++i];
	    } else {
	        fprintf(stderr, "Error: -f option requires a filename\n");
    		return 1;
	    }
	} else {
    	    csv_path = argv[i];
	}
    }

    if (csv_path == NULL) {
	#ifdef ADMIN_MODE
		fprintf(stderr, "Usage: ./admin_shell <csv_file> [-f command_file]\n");
	#elif defined(CLIENT_MODE)
		fprintf(stderr, "Usage: ./client_shell <csv_file> [-f command_file]\n");
	#endif
		return 1;
    }


#ifdef ADMIN_MODE
    /* Admin shell: supports add, delete, update, save, load, sort, list, find, help, exit */
    printf("[Admin Program]\n");
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#elif defined(CLIENT_MODE)
    /* Client shell: supports find, list, help, exit  (read-only) */
    printf("[Client Program]\n");
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#else
#error "Define either -DADMIN_MODE or -DCLIENT_MODE when compiling."
#endif

    return 0;
}
