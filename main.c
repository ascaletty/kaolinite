#include <strings.h>
#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "conio.h/conio.h"
#include "raylib/clay_renderer_raylib.c"
#include "raylib/raylib.h"
#include "zhash-c/src/zhash.h"
#include <regex.h>
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

Clay_Color COLOR_BACKGROUND = {38, 152, 152, 255};
Clay_Color COLOR_INPUT = {203, 4, 4, 230};
Clay_Color COLOR_HOVERED = {203, 4, 4, 255};
Clay_Color COLOR_EVEN = {194, 207, 178, 250};
Clay_Color COLOR_ODD = {126, 137, 135, 250};
GetPrograms() {
  execl("kaolinite_path.sh", "");
  FILE *fp = fopen("/home/ascaletty23/.cache/kaolinitedrun.cache", "r");
  if (!fp) {
    printf("CANT OPEN BOZO \n");
  } else {
    char buffer[1024];
    int row = 0;
    int collumn = 0;
    while (fgets(buffer, 1024, fp)) {
      collumn = 0;
      row++;
      if (row == 1) {
        continue;
      }
      char *value = strtok(buffer, ",");
      while (value) {
        if (collumn = 0) {
          printf("commonname:");
        }
        if (collumn == 1) {
          printf("\t exec:");
        }
        printf("%s", value);
        value = strtok(NULL, ", ");
        collumn++;
      }
      printf("\n");
    }
  }
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
  program.length = 20;
  program.isStaticallyAllocated = false;
  CLAY({.backgroundColor = Clay_Hovered() ? COLOR_HOVERED : COLOR_INPUT,
        .layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIT()}}}) {
    CLAY_TEXT(program, CLAY_TEXT_CONFIG({.fontId = 0,
                                         .fontSize = 36,
                                         .textColor = {255, 255, 255, 255}}));
  }
}
int search(char *str, const char *commands) {
  regex_t regex;
  int ret;
  ret = regcomp(&regex, str, 0);
  ret = regexec(&regex, commands, 0, NULL, 0);
  regfree(&regex);
  return ret == 0;
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
    // GetPrograms();
    for (int i = 0; i < NUM_OF_PROGRAMS; i++) {
      // int match= strcmp(commands[i],input);
      int match = search(input, commands[i]);
      if (match == 1) {
        printf("match found");
        execlp("kaolinite_run", commands[i]);
        // CloseWindow();
      } else {
        printf("match not found");
      }
    }
  }
  keepname.length = size;
  keepname.chars = input;
  keepname.isStaticallyAllocated = true;
  return keepname;
}

int main(void) {
  Clay_Raylib_Initialize(500, 400, "test", FLAG_WINDOW_RESIZABLE);
  // SetTargetFPS(2);
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
    Clay_SetPointerState((Clay_Vector2){mousePosition.x, mousePosition.y},
                         IsMouseButtonDown(0));

    // run every frame
    int key = GetCharPressed();
    int keyDown = 3;

    if (IsKeyPressed(KEY_BACKSPACE)) {
      keyDown = 0;
    } else if (IsKeyPressed(KEY_LEFT)) {
      keyDown = 1;
    } else if (IsKeyPressed(KEY_ENTER)) {
      keyDown = 2;
      printf("enter pressed");
    }
    Clay_String p1 = HandleTypinginput(key, keyDown);

    Clay_BeginLayout();
    CLAY({.id = CLAY_ID("OuterContainer"),
          .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM,
                     .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                     .padding = CLAY_PADDING_ALL(16),
                     .childGap = 5},
          .backgroundColor = COLOR_BACKGROUND}) {
      // text input here
      showProgram(p1);
      GetPrograms();
      // program list here

      for (int i = 0; i < NUM_OF_PROGRAMS; i++) {
        showProgramList(commands[i]);
      }
    }
    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    // raylib drawing begin here
    BeginDrawing();
    ClearBackground(BLACK);
    Clay_Raylib_Render(renderCommands, fonts);
    EndDrawing();
  }
  Clay_Raylib_Close();
}
