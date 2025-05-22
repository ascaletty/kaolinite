#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "raylib/clay_renderer_raylib.c"
#include "stdio.h"
#include "unistd.h"
#define MAX_CHARS 100
char input[MAX_CHARS+1]= "\0";
int letterCount= 1;


void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}
Clay_Color COLOR_BACKGROUND= {25, 116, 242, 255};
Clay_Color COLOR_INPUT= {221,110,15,255};
Clay_Color COLOR_EVEN= {194,207,178,250};
Clay_Color COLOR_ODD= {126,137,135,250};
void showProgram(Clay_String programName){
CLAY({
    .backgroundColor= COLOR_INPUT,
    .layout= {.sizing={.width=CLAY_SIZING_GROW()}}
  }) {
CLAY_TEXT(programName, CLAY_TEXT_CONFIG({ 
          .fontId = 0, 
          .fontSize = 36, 
          .textColor= {255,255,255,255}}));
  }
}
Clay_String HandleTypinginput(int key, bool keyDown){
  if(letterCount<=0){
    letterCount= 1;
  }
  if(letterCount>=100){
    input[0]= '\0';
    letterCount=0;
  }
  while((letterCount>=0) && (key>0)){
    input[letterCount]= (char) key;
    input[letterCount+1]= '\0';
  
    printf("Value of input = %c\n", input[letterCount]);
    printf("letterCount: %d\n", letterCount);
    Clay_String name;
    name.length= letterCount+1;
    //for(int i=0; i<letterCount+1;i++){
    name.chars= &input[letterCount];
    //printf("i value:%d\n",i);
    //}
    name.isStaticallyAllocated= true;
    letterCount++;
    return name;
  }
  Clay_String keepname;
  if(keyDown== true){
  letterCount--;
  input[letterCount]= '\0';  }
  keepname.length= letterCount+1;
  keepname.chars= &input[1];
  keepname.isStaticallyAllocated= true;
  return keepname; 
}


int main(void){
	Clay_Raylib_Initialize(500, 400, "test", FLAG_WINDOW_RESIZABLE);
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
    //run every frame
    int key=GetCharPressed(); 
    bool keyDown= false;
    if(IsKeyPressed(KEY_BACKSPACE)){
      keyDown= true;
    }
    Clay_String p1 =HandleTypinginput(key, keyDown);

	Clay_BeginLayout();
	CLAY({ .id = CLAY_ID("OuterContainer"), .layout = {.layoutDirection= CLAY_TOP_TO_BOTTOM, .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }, .backgroundColor = COLOR_BACKGROUND }){
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
}


