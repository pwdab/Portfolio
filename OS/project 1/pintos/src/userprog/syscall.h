#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

/* Process identifier. */
typedef int pid_t;
#define PID_ERROR ((pid_t) -1)

/* Map region identifier. */
typedef int mapid_t;
#define MAP_FAILED ((mapid_t) -1)

/* Maximum characters in a filename written by readdir(). */
#define READDIR_MAX_LEN 14

/* Typical return values from main() and arguments to exit(). */
#define EXIT_SUCCESS 0          /* Successful execution. */
#define EXIT_FAILURE 1          /* Unsuccessful execution. */

void syscall_init (void);
void halt (void);
void exit (int status);
pid_t exec (const char *file);
int wait (pid_t);
int read (int fd, void *buffer, unsigned length);
int write (int fd, const void *buffer, unsigned length);
int fibonacci(int n);
int max_of_four_int(int a, int b, int c, int d);

#endif /* userprog/syscall.h */
