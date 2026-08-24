#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char **items;
  size_t size;
  size_t capacity;
} TODOList;

char *read_task(void) {
  /* task nya realloc setiap habis*/
  size_t task_len = 0;
  size_t task_cap = 64;
  char *task = malloc(task_cap);
  int c;

  if (task == NULL) {
    return NULL;
  }

  while ((c = getchar()) != '\n' && c != EOF) {
    // printf("task len :%i| task cap: %i \n", (int)task_len, (int)task_cap);
    if (task_len + 1 >= task_cap) {
      task_cap *= 2;
      char *temp = realloc(task, task_cap);
      // printf("task len :%i| task cap: %i \n", (int)task_len, (int)task_cap);
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

int main(void) {
  TODOList tasks = {
      .items = NULL,
      .size = 0,
      .capacity = 8,
  };

  tasks.items = malloc(tasks.capacity * sizeof(*tasks.items));
  // printf("TODO cap: %d\n", (int)tasks.capacity);
  if (tasks.items == NULL) {
    fprintf(stderr, "Failed to allocate task list\n");
    return EXIT_FAILURE;
  }

  // get the task input
  char *task = read_task();
  if (task == NULL) {
    fprintf(stderr, "Failed to read input\n");
    return EXIT_FAILURE;
  }

  char **temp = expand_list(tasks.items, tasks.size, &tasks.capacity);
  if (temp == NULL) {
    fprintf(stderr, "Failed to grow task list\n");
    free(task);
    free(tasks.items);
    return EXIT_FAILURE;
  }

  tasks.items = temp;
  tasks.items[tasks.size++] = task;

  printf("--> %s\n", tasks.items[0]);

  // Cleanup
  for (size_t i = 0; i < tasks.size; i++) {
    free(tasks.items[i]);
  }
  free(tasks.items);

  return 0;
}
