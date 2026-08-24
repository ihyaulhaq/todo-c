#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char **items;
  size_t size;
  size_t capacity;
} TODOList;

char *read_task(void) {
  size_t task_len = 0;
  size_t task_cap = 64;
  char *task = malloc(task_cap);
  int c;

  if (task == NULL) {
    return NULL;
  }

  printf("add your task: ");
  while ((c = getchar()) != '\n' && c != EOF) {
    if (task_len + 1 >= task_cap) {
      task_cap *= 2;
      char *temp = realloc(task, task_cap);
      if (temp == NULL) {
        free(task);
        return NULL;
      }
      task = temp;
    }
    task[task_len++] = c;
  }
  task[task_len] = '\0';
  return task;
}

char **expand_list(char **list, size_t len, size_t *cap) {
  if (len < *cap) {
    return list;
  }
  size_t new_cap = *cap * 2;
  char **temp = realloc(list, new_cap * sizeof(*list));
  if (temp == NULL) {
    return NULL;
  }
  *cap = new_cap;
  return temp;
}

int add_list(TODOList *tasks) {
  // get the task input
  char *task = read_task();
  if (task == NULL) {
    fprintf(stderr, "Failed to read input\n");
    return 1;
  }

  // ecpand if the list not enough
  char **temp = expand_list(tasks->items, tasks->size, &tasks->capacity);
  if (temp == NULL) {
    fprintf(stderr, "Failed to grow task list\n");
    free(task);
    // free(tasks->items);
    return 1;
  }

  tasks->items = temp;
  tasks->items[tasks->size++] = task;

  for (size_t i = 0; i < tasks->size; i++) {
    printf("--> %s\n", tasks->items[i]);
  }

  return 0;
}

int main(void) {
  char buff[8];
  TODOList tasks = {
      .items = malloc(8 * sizeof(*tasks.items)),
      .size = 0,
      .capacity = 8,
  };

  if (tasks.items == NULL) {
    fprintf(stderr, "Failed to allocate task list\n");
    return 1;
  };

  printf("=========== Current List ===========\n");
  for (size_t i = 0; i < tasks.size; i++) {
    printf("-->%s\n", tasks.items[i]);
  }
  printf("\n[a] Add task\n");
  printf("[q] Quit\n");
  printf("[l] List\n");
  while (1) {
    printf("> ");

    char *command = fgets(buff, sizeof(buff), stdin);
    if (command == NULL)
      break;
    command[strcspn(command, "\n")] = '\0';

    if (strcmp(command, "a") == 0) {
      add_list(&tasks);

    } else if (strcmp(command, "l") == 0) {
      printf("=========== Current List ===========\n");
      if (tasks.size == 0) {
        printf("=========== Empty ===========\n");
      } else {
        for (size_t i = 0; i < tasks.size; i++) {
          printf("-->%s\n", tasks.items[i]);
        }
      }

    } else if (strcmp(command, "q") == 0) {
      break;

    } else {
      printf("command doesn't exist\n");
    }
  }

  for (size_t i = 0; i < tasks.size; i++) {
    free(tasks.items[i]);
  }
  free(tasks.items);

  return 0;
}
