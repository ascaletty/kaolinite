#include <strings.h>
#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "raylib/clay_renderer_raylib.c"
#include "raylib/raylib.h"
#include "uthash/src/uthash.h"
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_CHARS 10
#define NUM_OF_PROGRAMS 4
char input[MAX_CHARS + 1];
int CursorPos = 0;
int size = 1;
char commands[NUM_OF_PROGRAMS][20] = {
    {"firefox"}, {"kitty"}, {"rofi"}, {"nvim"}};

struct posSize {
  int pos, size;
};
typedef struct posSize Struct;
int indexofCommands[NUM_OF_PROGRAMS];
void HandleClayErrors(Clay_ErrorData errorData) {
  printf("%s", errorData.errorText.chars);
}

// colors
Clay_Color COLOR_BACKGROUND = {38, 152, 152, 255};
Clay_Color COLOR_INPUT = {203, 4, 4, 230};
Clay_Color COLOR_HOVERED = {203, 4, 4, 255};
Clay_Color COLOR_EVEN = {194, 207, 178, 250};
Clay_Color COLOR_ODD = {126, 137, 135, 250};
struct hashtable {
  char id[1048]; /* key */
  char exec[1048];
  UT_hash_handle hh; /* makes this structure hashable */
};

struct hashtable *users = NULL;
void add_user(char *id, const char *exec) {
  struct hashtable *s;

  HASH_FIND_INT(users, id, s); /* id already in the hash? */
  if (s == NULL) {
    s = (struct hashtable *)malloc(sizeof *s);
    strcpy(s->id, id);
    HASH_ADD_INT(users, id, s); /* id is the key field */
  }
  strcpy(s->exec, exec);
}

struct hashtable *find_user(char *user_id) {
  struct hashtable *s;

  HASH_FIND_INT(users, user_id, s); /* s: output pointer */
  return s;
}
void delete_all() {
  struct hashtable *current_user;
  struct hashtable *tmp;

  HASH_ITER(hh, users, current_user, tmp) {
    HASH_DEL(users, current_user); /* delete it (users advances to next) */
    free(current_user);            /* free it */
  }
}

int GetCommands() {
  char buffer[1048];
  system("kaolinite_path");
  FILE *fp;
  fp = fopen("/home/ascaletty23/.cache/kaolinitedrun.cache", "r");
  while (fgets(buffer, 1048, fp)) {
    char *token = strtok(buffer, ",");
    char *tokenarray[2];
    int column = 0;
    while (token) {
      if (column == 0) {
        tokenarray[0] = token;
      } else {
        tokenarray[1] = token;
      }
      token = strtok(NULL, ",");
      column++;
    }
    add_user(*tokenarray, *(tokenarray + 1));
  }
  return 0;
}
int insertText(char input[], int size, int pos, int val) {
  input[pos] = val;
  size++;
  CursorPos++;
  return size;
}
Struct backspace(char input[], int size, int pos) {

  if (pos <= 0) {
    pos = 0;
  }

  pos--;
  input[pos] = '\0';
  size--;
  Struct s;
  s.size = size;
  s.pos = pos;
  return s;
}
void showProgram(Clay_String programName) {
  CLAY({
      .backgroundColor = Clay_Hovered() ? COLOR_HOVERED : COLOR_INPUT,
      .layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIT()}},
  }) {
    CLAY_TEXT(
        programName,
        CLAY_TEXT_CONFIG(
            {.fontId = 0, .fontSize = 36, .textColor = {255, 255, 255, 255}}));
  }
}
void showProgramList(char *programAddress) {
  Clay_String program;
  program.chars = programAddress;
  program.length = 1048;
  program.isStaticallyAllocated = true;
  CLAY({.backgroundColor = Clay_Hovered() ? COLOR_HOVERED : COLOR_INPUT,
        .layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(40)}}}) {
    CLAY_TEXT(program, CLAY_TEXT_CONFIG({.fontId = 0,
                                         .fontSize = 36,
                                         .textColor = {255, 255, 255, 255}}));
  }
}
void print_users() {
  struct hashtable *s;
  // printf("print_users called");
  for (s = users; s != NULL; s = (struct hashtable *)(s->hh.next)) {
    // printf("user id %s: name %s\n", s->id, s->exec);
    showProgramList(s->id);
  }
}

int search(char *str, char *s) {
  regex_t regex;
  printf(" String passed to search %s\n", str);
  int ret;
  ret = regcomp(&regex, str, REG_ICASE);
  ret = regexec(&regex, s, 0, NULL, 0);
  regfree(&regex);
  return ret == 1;
}

int by_string(struct hashtable *a, struct hashtable *b) {
  printf(" input passed to by_string%s\n", input);
  return search(input, a->id);
}

void exec_program(char *input) {
  struct hashtable *s = users;
  // for (s; s != NULL; s = (struct hashtable *)(s->hh.next)) {
  printf("user id %s: name %s\n", s->id, s->exec);
  // int match = search(input, s->id);
  // if (match == 1) {
  printf("match found");
  struct hashtable *currentprogram = find_user(s->id);
  printf("currentprogam %s", currentprogram->exec);
  execlp("kaolinite_run", currentprogram->exec);
  // CloseWindow();
  // } else {
  //   printf("match not found \n");
  // }
  // }
}
Clay_String HandleTypinginput(int key, int keyDown) {
  if (CursorPos <= 0) {
    CursorPos = 0;
  }
  if (CursorPos >= 100) {
    CursorPos = 0;
  }
  if (size <= 0) {
    size = 1;
  }
  while ((CursorPos >= 0) && (key > 0)) {
    Clay_String name;
    input[CursorPos] = (char)key;
    size = insertText(input, size, CursorPos, input[CursorPos]);
    printf("size %d\n", size);
    name.length = size;
    name.chars = input;
    name.isStaticallyAllocated = false;
    HASH_SORT(users, by_string);
    return name;
  }
  Clay_String keepname;

  // when backspace is pressed
  if (keyDown == 0) {
    Struct context;
    context = backspace(input, size, CursorPos);
    CursorPos = context.pos;
    size = context.size;
    printf("size %d\n", size);
    printf("CursorPos %d\n", CursorPos);

  } else if (keyDown == 2) {
    exec_program(input);
  }
  keepname.length = size;
  keepname.chars = input;
  keepname.isStaticallyAllocated = true;
  return keepname;
}

int main() {
  printf("getcommans return val:%d\n", GetCommands());

  Clay_Raylib_Initialize(500, 400, "test", FLAG_WINDOW_RESIZABLE);
  uint64_t clayRequiredMemory = Clay_MinMemorySize();
  Clay_Arena clayMemory = (Clay_Arena){.memory = malloc(clayRequiredMemory),
                                       .capacity = clayRequiredMemory};
  Clay_Initialize(
      clayMemory,
      (Clay_Dimensions){.width = GetScreenWidth(), .height = GetScreenHeight()},
      (Clay_ErrorHandler){HandleClayErrors});
  Font fonts[1];
  fonts[0] = LoadFontEx("/usr/share/fonts/TTF/FiraCode-Retina.ttf", 48, 0, 400);
  Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);
  while (!WindowShouldClose()) {
    Vector2 mousePosition = GetMousePosition();
    Vector2 scrollDelta = GetMouseWheelMoveV();
    Clay_SetPointerState((Clay_Vector2){mousePosition.x, mousePosition.y},
                         IsMouseButtonDown(0));
    Clay_UpdateScrollContainers(
        false, (Clay_Vector2){scrollDelta.x, scrollDelta.y}, GetFrameTime());

    // run every frame
    int key = GetCharPressed();
    int keyDown = 3;
    if (IsKeyPressed(KEY_BACKSPACE)) {
      keyDown = 0;
    } else if (IsKeyPressed(KEY_LEFT)) {
      keyDown = 1;
    } else if (IsKeyPressed(KEY_ENTER)) {
      keyDown = 2;
    }

    Clay_String p1 = HandleTypinginput(key, keyDown);

    Clay_BeginLayout();
    CLAY({.id = CLAY_ID("OuterContainer"),
          .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM,
                     .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                     .padding = CLAY_PADDING_ALL(16),
                     .childGap = 5},
          .clip =
              {
                  .vertical = true,
                  .childOffset = Clay_GetScrollOffset(),
              },
          .backgroundColor = COLOR_BACKGROUND}) {
      // text input here
      showProgram(p1);
      // program list here
      print_users();
    }
    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    // raylib drawing begin here
    BeginDrawing();
    ClearBackground(BLACK);
    Clay_Raylib_Render(renderCommands, fonts);
    EndDrawing();
  }
  Clay_Raylib_Close();
  delete_all();
}
