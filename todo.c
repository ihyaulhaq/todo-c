#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *text;
  bool is_done;
  size_t size;
  size_t capacity;
} TODOItem;

typedef struct {
  TODOItem *items;
  size_t size;
  size_t capacity;
} TODOList;

TODOItem read_task(void) {
  int c;
  TODOItem task = {
      .size = 0,
      .capacity = 64,
      .text = NULL,
      .is_done = false,
  };

  task.text = malloc(task.capacity);
  if (task.text == NULL) {
    return task;
  }

  printf("add your task: ");
  while ((c = getchar()) != '\n' && c != EOF) {
    if (task.size + 1 >= task.capacity) {
      task.capacity *= 2;
      char *temp = realloc(task.text, task.capacity);
      if (temp == NULL) {
        free(task.text);
        task.text = NULL;
        task.size = 0;
        task.capacity = 0;
        return task;
      }
      task.text = temp;
    }
    task.text[task.size++] = c;
  }
  task.text[task.size] = '\0';
  return task;
}

void print_list(const TODOList *tasks) {
  printf("\n=========== Current List ===========\n");
  if (tasks->size == 0) {
    printf("\n=========== Empty ===========\n");
  } else {
    for (size_t i = 0; i < tasks->size; i++) {
      printf("%zu. %s [%c]\n", i + 1, tasks->items[i].text,
             tasks->items[i].is_done ? '#' : ' ');
    }
  }
}

TODOItem *expand_list(TODOItem *list, size_t len, size_t *cap) {
  if (len < *cap) {
    return list;
  }
  size_t new_cap = *cap * 2;
  TODOItem *temp = realloc(list, new_cap * sizeof(*list));
  if (temp == NULL) {
    return NULL;
  }
  *cap = new_cap;
  return temp;
}

int add_task(TODOList *tasks) {
  // get the task input
  TODOItem task = read_task();
  if (task.text == NULL) {
    fprintf(stderr, "Failed to read input\n");
    return 1;
  }

  // ecpand if the list not enough
  TODOItem *temp = expand_list(tasks->items, tasks->size, &tasks->capacity);
  if (temp == NULL) {
    fprintf(stderr, "Failed to grow task list\n");
    free(task.text);
    return 1;
  }

  tasks->items = temp;
  tasks->items[tasks->size++] = task;
  print_list(tasks);
  return 0;
}

int delete_task(TODOList *tasks) {
  if (tasks->size == 0) {
    printf("list empty\n");
    return 1;
  }
  char buff[8];
  printf("input number task to delete it\n> ");
  if (fgets(buff, sizeof(buff), stdin) == NULL) {
    return 1;
  }
  int idx = atoi(buff) - 1;
  if (idx < 0 || (size_t)idx >= tasks->size) {
    printf("invalid number\n");
    return 1;
  }

  free(tasks->items[idx].text);
  for (size_t i = idx; i + 1 < tasks->size; i++) {
    tasks->items[i] = tasks->items[i + 1];
  }
  tasks->size--;

  printf("success delete task number %d\n", idx + 1);
  print_list(tasks);
  return 0;
}

int mark_done(TODOList *tasks) {
  if (tasks->size == 0) {
    printf("list empty\n");
    return 1;
  }
  char buff[8];
  printf("input number task to mark it\n> ");
  if (fgets(buff, sizeof(buff), stdin) == NULL) {
    return 1;
  }
  int idx = atoi(buff) - 1;
  if (idx < 0 || (size_t)idx >= tasks->size) {
    printf("invalid number\n");
    return 1;
  }

  tasks->items[idx].is_done = true;

  print_list(tasks);

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

  print_list(&tasks);
  printf("\n[a] Add task\n");
  printf("[d] delete task\n");
  printf("[m] mark task as done\n");
  printf("[q] Quit\n");
  printf("[l] List\n");

  while (1) {
    printf("\n> ");

    char *command = fgets(buff, sizeof(buff), stdin);
    if (command == NULL)
      break;
    command[strcspn(command, "\n")] = '\0';

    if (strcmp(command, "a") == 0) {
      add_task(&tasks);

    } else if (strcmp(command, "m") == 0) {
      mark_done(&tasks);

    } else if (strcmp(command, "d") == 0) {
      delete_task(&tasks);

    } else if (strcmp(command, "l") == 0) {
      print_list(&tasks);

    } else if (strcmp(command, "q") == 0) {
      break;

    } else {
      printf("command doesn't exist\n");
    }
  }

  for (size_t i = 0; i < tasks.size; i++) {
    free(tasks.items[i].text);
  }
  free(tasks.items);

  return 0;
}
