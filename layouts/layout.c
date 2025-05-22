include "../clay.h"
#include <stdlib.h>
const int FONT_ID_BODY_16 = 0;
Clay_Color COLOR_BACKGROUND = {4, 59, 92, 255};
Clay_RenderCommandArray Clay_CreateLayout(Clay_Data *data){data->frameArena.offset = 0;
Clay_BeginLayout();
	Clay_Sizing layoutExpand = {
	.width= CLAY_SIZING_GROW(0),
	.height = CLAY_SIZING_GROW(0)
	}
 CLAY({ .id = CLAY_ID("OuterContainer"),
        .backgroundColor = {43, 41, 51, 255 },
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = layoutExpand,
            .padding = CLAY_PADDING_ALL(16),
            .childGap = 16
        }
    });
 Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    for (int32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommandArray_Get(&renderCommands, i)->boundingBox.y += data->yOffset;
    }
    return renderCommands;
}
}
