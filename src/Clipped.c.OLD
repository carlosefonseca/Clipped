#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xB4, 0x6D, 0x25, 0x42, 0x6F, 0x7D, 0x46, 0x2C, 0x87, 0x9B, 0xE2, 0x1A, 0xE0, 0xDD, 0x69, 0xEF }
PBL_APP_INFO(MY_UUID,
             "Clipped", "Jnm",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define CX 72
#define CY 84
#define SCREENW 144
#define SCREENH 168
#define TEXTX 0
#define TEXTY 110
#define TEXTW 143
#define TEXTH 60

const int hourImage[10] = {
    RESOURCE_ID_IMAGE_H0, RESOURCE_ID_IMAGE_H1, RESOURCE_ID_IMAGE_H2, RESOURCE_ID_IMAGE_H3,
    RESOURCE_ID_IMAGE_H4, RESOURCE_ID_IMAGE_H5, RESOURCE_ID_IMAGE_H6, RESOURCE_ID_IMAGE_H7,
    RESOURCE_ID_IMAGE_H8, RESOURCE_ID_IMAGE_H9
};

Window window;
Layer layer;
TextLayer minuteLayerBlack, minuteLayerWhite[4];
GFont minuteFont;
char minutes[] = "  ";

int h1, h2, m1, m2;

BmpContainer hourBmpContainer[10];

void updateLayer(Layer *me, GContext* ctx) {
    GRect frameH1, frameH2;
    int hourWidth;
    
    frameH1 = layer_get_frame(&hourBmpContainer[h1].layer.layer);
    frameH2 = layer_get_frame(&hourBmpContainer[h2].layer.layer);
    hourWidth = frameH1.size.w + frameH2.size.w + 8;
    
    // tens hour digit
    frameH1.origin.x = CX - hourWidth/2;
    frameH1.origin.y = CY - frameH1.size.h/2;
    graphics_draw_bitmap_in_rect(ctx, &hourBmpContainer[h1].bmp, frameH1);
    
    // units hour digit
    frameH2.origin.x = frameH1.origin.x + frameH1.size.w + 4;
    frameH2.origin.y = CY - frameH2.size.h/2;
    graphics_draw_bitmap_in_rect(ctx, &hourBmpContainer[h2].bmp, frameH2);
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *evt) {
    PblTm now;
    get_time(&now);

    h1 = now.tm_hour/10;
    h2 = now.tm_hour%10;
    m1 = now.tm_min/10;
    m2 = now.tm_min%10;

    minutes[0] = '0' + (char)m1;
    minutes[1] = '0' + (char)m2;
    text_layer_set_text(&minuteLayerBlack, minutes);
    layer_mark_dirty(&layer);
}

void handle_init(AppContextRef ctx) {
    PblTm now;
    int i;
	int dx[4] = { -1, 1, 1, -1 };
	int dy[4] = { -1, -1, 1, 1 };
   
    window_init(&window, "Clipped");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorBlack);
    
    resource_init_current_app(&APP_RESOURCES);

    //minuteFont = fonts_get_system_font(FONT_KEY_GOTHAM_42_MEDIUM_NUMBERS);
    minuteFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BORIS_40));
    
    get_time(&now);
    
    h1 = now.tm_hour/10;
    h2 = now.tm_hour%10;
    m1 = now.tm_min/10;
    m2 = now.tm_min%10;

    for (i=0; i<10; i++) {
        bmp_init_container(hourImage[i], &hourBmpContainer[i]);
    }
    
    layer_init(&layer, window.layer.frame);
    layer.update_proc = &updateLayer;
    layer_add_child(&window.layer, &layer);
    
	for (i=0; i<4; i++) {
    	text_layer_init(&minuteLayerWhite[i], GRect(TEXTX+dx[i], TEXTY+dy[i], TEXTW, TEXTH));
    	text_layer_set_background_color(&minuteLayerWhite[i], GColorClear);
    	text_layer_set_font(&minuteLayerWhite[i], minuteFont);
    	text_layer_set_text_alignment(&minuteLayerWhite[i], GTextAlignmentRight);
    	text_layer_set_text_color(&minuteLayerWhite[i], GColorWhite);
    	minutes[0] = '0' + (char)m1;
    	minutes[1] = '0' + (char)m2;
    	text_layer_set_text(&minuteLayerWhite[i], minutes);
    	layer_add_child(&layer, &minuteLayerWhite[i].layer);
	}

    text_layer_init(&minuteLayerBlack, GRect(TEXTX, TEXTY, TEXTW, TEXTH));
    text_layer_set_background_color(&minuteLayerBlack, GColorClear);
    text_layer_set_font(&minuteLayerBlack, minuteFont);
    text_layer_set_text_alignment(&minuteLayerBlack, GTextAlignmentRight);
    text_layer_set_text_color(&minuteLayerBlack, GColorBlack);
    minutes[0] = '0' + (char)m1;
    minutes[1] = '0' + (char)m2;
    text_layer_set_text(&minuteLayerBlack, minutes);
    layer_add_child(&layer, &minuteLayerBlack.layer);
    
}

void handle_deinit(AppContextRef ctx) {
    int i;
        
    for (i=0; i<10; i++) {
        bmp_deinit_container(&hourBmpContainer[i]);
    }
}

void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
        .init_handler = &handle_init,
        .deinit_handler = &handle_deinit,
        
		.tick_info = {
			.tick_handler = &handle_tick,
			.tick_units   = MINUTE_UNIT
		}
    };
    app_event_loop(params, &handlers);
}
