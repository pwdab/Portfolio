#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

static void syscall_handler (struct intr_frame *);

void syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void check_address(const void *vaddr)
{
  //if(!is_user_vaddr(vaddr) || pagedir_get_page(thread_current ()->pagedir, vaddr) == NULL)
  if(!is_user_vaddr(vaddr) || is_kernel_vaddr(vaddr) || pagedir_get_page(thread_current ()->pagedir, vaddr) == NULL)
    exit(-1);
}

static void get_argument(void *esp, int *arg, int count)
{
	int idx = 0;
	void *ptr = esp + 4;

	while(count--)
	{
		check_address(ptr);
		arg[idx++] = *(int *)ptr;
		ptr += 4;
	}
}

static void syscall_handler (struct intr_frame *f)
{
  int arg[5];

  check_address(f->esp);
  
  void *esp = f->esp;
  uint32_t NUMBER = *(uint32_t *)esp;

  /* Debugging */
  /*
  int byte_size = 0xc0000000 - (int)f->esp;
  printf("esp = %p, eip = %p\n", esp, f->eip);
  printf("syscall : %d, byte_size = %d\n",*(uint32_t *)(f->esp), byte_size);
  hex_dump((uintptr_t)f->esp, f->esp, byte_size, 1);
  */

  switch(NUMBER) {
    case SYS_HALT:
      halt();
      break;
    case SYS_EXIT:
      get_argument(esp, arg, 1);
      exit((int)arg[0]);
      break;
    case SYS_EXEC:
      get_argument(esp, arg, 1);
      f->eax = exec((const char*)arg[0]);
      break;
    case SYS_WAIT:
      get_argument(esp, arg, 1);
      f->eax = wait((int)arg[0]);
      break;
    case SYS_READ:
      get_argument(esp, arg, 3);
      f->eax = read((int)arg[0], (void*)arg[1], (unsigned)arg[2]);
      break;
    case SYS_WRITE:
      get_argument(esp, arg, 3);
      f->eax = write((int)arg[0], (void*)arg[1], (unsigned)arg[2]);
      break;
    case SYS_FIBONACCI:
      get_argument(esp, arg, 1);
      f->eax = fibonacci((int)arg[0]);
      break;
    case SYS_MAX_OF_FOUR:
      get_argument(esp, arg, 4);
      f->eax = max_of_four_int((int)arg[0], (int)arg[1], (int)arg[2], (int)arg[3]);
      break;
  }
}

void halt(void)
{
  shutdown_power_off();
}

void exit (int status)
{
  printf("%s: exit(%d)\n", thread_name(), status);
  thread_current()->exit_status = status;
  thread_exit();
}

pid_t exec (const char *file)
{
  check_address(file);
  return process_execute(file);
}

int wait (pid_t pid)
{
  return process_wait(pid);
}

int read (int fd, void* buffer, unsigned size) {
  check_address(buffer);

  int result = -1;
  if (fd == 0) {
    *(char *)buffer = input_getc();
      result = size;
  }

  return result;
}

int write (int fd, const void *buffer, unsigned size)
{
  check_address(buffer);

  //printf("fd = %d, buffer = %p, size = %d\n", fd, buffer, size);
  switch(fd) {
    case STDIN_FILENO:
      break;
    case STDOUT_FILENO:
      putbuf(buffer, size);
      return size;
      break;
    case STDERR_FILENO:
      break;
  }
  return -1;
}

int fibonacci(int n)
{
  if(n <= 1)
    return n;

  return fibonacci(n - 1) + fibonacci(n - 2);
}

int max_of_four_int(int a, int b, int c, int d)
{
  int max = a;

  if(max < b)
    max = b;
  if(max < c)
    max = c;
  if(max < d)
    max = d;

  return max;
}