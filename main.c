#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threads.h>
#include <stdbool.h>

bool scp_done = false;

int show_progress(void *arg);

int main(int argc, char *argv[]) {
    if (argc < 5 || argc > 5) {
        printf("\n");
        printf("Usage: <this_executable_c_file> <ip> <password> <remote_path> <local_path>\n");
        return 1;
    }
    else {
        char executable_string[512];
        snprintf(executable_string, sizeof(executable_string), "sshpass -p '%s' scp -r root@%s:%s %s ", argv[2], argv[1], argv[3], argv[4]);

        thrd_t progress_thread;
        thrd_create(&progress_thread, show_progress, NULL);

        int error = system(executable_string);

        scp_done = true;

        thrd_join(progress_thread, NULL);

        if (!error) {
            printf(stderr, "Finished with code: %s\n", error);
        }
    }
    return 0;
}

int show_progress(void *arg) {
    const char *spinner = "|/-\\";
    int i = 0;
    while (!scp_done) {
        printf("\rProcessing... %c", spinner[i % 4]);
        fflush(stdout);
        usleep(200000); // 0.2 сек
        i++;
    }
    printf("\rAll done!\n");
    return 0;
}
