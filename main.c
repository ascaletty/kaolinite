#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "raylib/clay_renderer_raylib.c"
#include "raylib/raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define MAX_CHARS 10
char input[MAX_CHARS+1];
int CursorPos= 0;
int size=1;
const char commands[3][20]= {
  {"firefox"},
    {"kitty"},
    {"rofi"}
};

struct greaterSmaller {
    int pos, size;
};
typedef struct greaterSmaller Struct;
void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}


Clay_Color COLOR_BACKGROUND= {38, 152, 152, 255};
Clay_Color COLOR_INPUT= {203,4,4,230};
Clay_Color COLOR_HOVERED= {203,4,4,255};
Clay_Color COLOR_EVEN= {194,207,178,250};
Clay_Color COLOR_ODD= {126,137,135,250};

int insertText(char input[], int size, int pos, int val){
  input[pos]= val;
  size++;
  return size;
}
Struct backspace(char input[], int size, int pos){
   if(CursorPos<=0){
    CursorPos= 0;
  }
  input[pos]= '\0';
  pos--;
  size--;
Struct s;
  s.size= size;
  s.pos= pos;
  return s;
}
void showProgram(Clay_String programName){
CLAY({
    .id= CLAY_ID("INPUT"),
    .backgroundColor= Clay_Hovered() ? COLOR_HOVERED: COLOR_INPUT,
    .layout= {.sizing={.width=CLAY_SIZING_GROW()}}
  }) {
CLAY_TEXT(programName, CLAY_TEXT_CONFIG({ 
          .fontId = 0, 
          .fontSize = 36, 
          .textColor= {255,255,255,255}}));
  }
}
Clay_String HandleTypinginput(int key,int keyDown){
  if(CursorPos<=0){
    CursorPos= 0;
  }
  if(CursorPos>=100){
    CursorPos=0;
  }
  if(size<=0){
    size=1;
  }
  while((CursorPos>=0) && (key>0)){
     Clay_String name;
    input[CursorPos]= (char) key;
    //input[CursorPos+1]= '\0';
   // printf("input length :%d", strlen(input));
    size=insertText(input, size, CursorPos, input[CursorPos]);
    printf("size %d\n", size);
    name.length= size;
    // name.chars= &input[CursorPos];
    name.chars= input;
    name.isStaticallyAllocated= false;
    CursorPos++;
    return name;
  }
  Clay_String keepname;

  //when backspace is pressed 
  if(keyDown==0){
  Struct context;
  context = backspace(input,size,CursorPos);
  CursorPos= context.pos;
  size= context.size;
  printf("size %d\n",size);
  printf("CursorPos %d\n",CursorPos);

  }
  else if(keyDown==2){
    char cmd[size+2];
    // strncpy(cmd,input,size+2);
   for(int i=0; i<size+2; i++){
      printf("char %d :%c\n", i, *(input+i));
    }
for(int i=0; i<3; i++){
 // int match= strcmp(commands[i],input);
  printf("command[ %d\n]: %s\n", i, commands[i]);
  printf("output %d\n", strcmp(input,commands[i]));
int match =  strcmp(input,commands[i]);
 if(match==0){
    printf("match found");
    system(commands[i]);
   CloseWindow(); 
  }
  else{
   printf("match not found");
 }
    }
  }
  keepname.length= size;
  keepname.chars= input;
  keepname.isStaticallyAllocated= true;
  return keepname;
}


int main(void){
	Clay_Raylib_Initialize(500, 400, "test", FLAG_WINDOW_RESIZABLE);
  //SetTargetFPS(2);
	uint64_t clayRequiredMemory= Clay_MinMemorySize();
	Clay_Arena clayMemory = (Clay_Arena) {
		.memory = malloc(clayRequiredMemory),
    .capacity = clayRequiredMemory
	};
	Clay_Initialize(clayMemory, (Clay_Dimensions){
	.width = GetScreenWidth(),
	.height= GetScreenHeight()
	},(Clay_ErrorHandler) {HandleClayErrors});
	Font fonts[1];
	fonts[0] = LoadFontEx("resources/Roboto-Regular.ttf", 48, 0, 400);
	Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);
	while(!WindowShouldClose()){ 
      Vector2 mousePosition = GetMousePosition();
      Clay_SetPointerState(
      (Clay_Vector2) {mousePosition.x,mousePosition.y},
      IsMouseButtonDown(0)
      );

    //run every frame
    int key=GetCharPressed(); 
    int keyDown=3;

    if(IsKeyPressed(KEY_BACKSPACE)){
      keyDown= 0;
    }
    else if(IsKeyPressed(KEY_LEFT)){
      keyDown=1;
    }
    else if(IsKeyPressed(KEY_ENTER)){
      keyDown=2;
    }
    Clay_String p1 =HandleTypinginput(key, keyDown);

  Clay_BeginLayout();
	CLAY({ .id = CLAY_ID("OuterContainer"), 
      .layout = {.layoutDirection= CLAY_TOP_TO_BOTTOM, 
      .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, 
        .padding = CLAY_PADDING_ALL(16), .childGap = 16 }, 
        .backgroundColor = COLOR_BACKGROUND }){
      {
        showProgram(p1);
      }
     /// Programlist here
	}
	Clay_RenderCommandArray renderCommands= Clay_EndLayout();
	//raylib drawing begin here
	BeginDrawing();
	ClearBackground(BLACK);
	Clay_Raylib_Render(renderCommands, fonts);
	EndDrawing();
	}
  Clay_Raylib_Close();
}


