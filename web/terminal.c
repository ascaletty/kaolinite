#define CLAY_IMPLEMENTATION
#include "../clay/clay.h"
#include "../raylib/clay_renderer_raylib.c"

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}
Clay_Color COLOR_BACKGROUND= {25, 116, 242, 255};


int main(void){
	Clay_Raylib_Initialize(500, 100, "test", FLAG_WINDOW_RESIZABLE);
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
	Clay_BeginLayout();
	CLAY({ .id = CLAY_ID("OuterContainer"), .layout = {.layoutDirection= CLAY_TOP_TO_BOTTOM, .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }, .backgroundColor = COLOR_BACKGROUND }){
	CLAY({ .id = CLAY_ID("Textbox"), 
			.layout = { 
				.sizing = {
					.height= CLAY_SIZING_FIXED(60),
					.width= CLAY_SIZING_GROW()
					},
				.padding= { 0, 0, 0 , 0},
				.childAlignment= {
				.y= CLAY_ALIGN_Y_CENTER,
				.x= CLAY_ALIGN_X_LEFT
				}
			},
			.backgroundColor = { 0, 0, 0, 255},
			.cornerRadius= CLAY_CORNER_RADIUS(8)
			}) {
	CLAY_TEXT(CLAY_STRING(">"), CLAY_TEXT_CONFIG({ .fontId = 0, .fontSize = 36, .textColor= {255,255,255,255}}));
	}	
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


