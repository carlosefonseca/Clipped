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
#define TEXTY 95
#define TEXTW 143
#define TEXTH 60
#define USDATE false
#define WEEKDAY true

const int hourImage[10] = {
    RESOURCE_ID_IMAGE_H0, RESOURCE_ID_IMAGE_H1, RESOURCE_ID_IMAGE_H2, RESOURCE_ID_IMAGE_H3,
    RESOURCE_ID_IMAGE_H4, RESOURCE_ID_IMAGE_H5, RESOURCE_ID_IMAGE_H6, RESOURCE_ID_IMAGE_H7,
    RESOURCE_ID_IMAGE_H8, RESOURCE_ID_IMAGE_H9
};


//const char weekDay[7][3] = { "Zo", "Ma", "Di", "Wo", "Do", "Vr", "Za" };	// Dutch
const char weekDay[7][3] = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };	// English
//const char weekDay[7][3] = { "Di", "Lu", "Ma", "Me", "Je", "Ve", "Sa" };	// French
//const char weekDay[7][3] = { "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa" };	// German
//const char weekDay[7][3] = { "Do", "Lu", "Ma", "Mi", "Ju", "Vi", "Sa" };	// Spanish

Window window;
Layer bgLayer;
TextLayer minuteLayer[5], dateLayer;
GFont minuteFont;
char minutes[] = "01";
char date[] = "01 34";
int h1, h2, m1, m2, D1, D2, M1, M2, wd;
bool clock12 = false;
int dx[5] = { -1, 1, 1, -1, 0 };
int dy[5] = { -1, -1, 1, 1, 0 };
GColor textColor[5] = { GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorBlack };
BmpContainer hourBmpContainer[2];

void updateLayer(Layer *me, GContext* ctx) {
    GRect frameH1, frameH2;
    int hourWidth;
    
	bmp_init_container(hourImage[h1], &hourBmpContainer[0]);
	bmp_init_container(hourImage[h2], &hourBmpContainer[1]);

    frameH1 = layer_get_frame(&hourBmpContainer[0].layer.layer);
    frameH2 = layer_get_frame(&hourBmpContainer[1].layer.layer);
    hourWidth = frameH1.size.w + frameH2.size.w + 8;
    
    // tens hour digit
    frameH1.origin.x = CX - hourWidth/2;
    frameH1.origin.y = 1;
    graphics_draw_bitmap_in_rect(ctx, &hourBmpContainer[0].bmp, frameH1);
    
    // units hour digit
    frameH2.origin.x = frameH1.origin.x + frameH1.size.w + 4;
    frameH2.origin.y = 1;
    graphics_draw_bitmap_in_rect(ctx, &hourBmpContainer[1].bmp, frameH2);

	bmp_deinit_container(&hourBmpContainer[0]);
	bmp_deinit_container(&hourBmpContainer[1]);
}

void setHM(PblTm *tm) {
	int h = tm->tm_hour;
		if (clock12) {
			h = h%12;
		if (h == 0) {
			h = 12;
		}
	}
    h1 = h/10;
    h2 = h%10;
    m1 = tm->tm_min/10;
    m2 = tm->tm_min%10;
    D1 = tm->tm_mday/10;
    D2 = tm->tm_mday%10;
    M1 = (tm->tm_mon+1)/10;
    M2 = (tm->tm_mon+1)%10;
	wd = tm->tm_wday;

    minutes[0] = '0' + (char)m1;
    minutes[1] = '0' + (char)m2;

	if (WEEKDAY) {
			date[0] = weekDay[wd][0];
			date[1] = weekDay[wd][1];
			date[3] = '0' + (char)D1;
			date[4] = '0' + (char)D2;
	} else {
		if (USDATE) {
			date[0] = '0' + (char)M1;
			date[1] = '0' + (char)M2;
			date[3] = '0' + (char)D1;
			date[4] = '0' + (char)D2;
		} else {
			date[0] = '0' + (char)D1;
			date[1] = '0' + (char)D2;
			date[3] = '0' + (char)M1;
			date[4] = '0' + (char)M2;
		}
	}
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *evt) {
    PblTm now;

    get_time(&now);
	setHM(&now);
    layer_mark_dirty(&bgLayer);
}

void handle_init(AppContextRef ctx) {
    PblTm now;
    int i;
   
    window_init(&window, "Clipped");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorBlack);
    
    resource_init_current_app(&APP_RESOURCES);

    minuteFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BORIS_40));
    
	clock12 = !clock_is_24h_style();

    get_time(&now);
	setHM(&now);

    layer_init(&bgLayer, window.layer.frame);
    bgLayer.update_proc = &updateLayer;
    layer_add_child(&window.layer, &bgLayer);
    
	for (i=0; i<5; i++) {
    	text_layer_init(&minuteLayer[i], GRect(TEXTX+dx[i], TEXTY+dy[i], TEXTW, TEXTH));
    	text_layer_set_background_color(&minuteLayer[i], GColorClear);
    	text_layer_set_font(&minuteLayer[i], minuteFont);
    	text_layer_set_text_alignment(&minuteLayer[i], GTextAlignmentRight);
    	text_layer_set_text_color(&minuteLayer[i], textColor[i]);
    	text_layer_set_text(&minuteLayer[i], minutes);
    	layer_add_child(&bgLayer, &minuteLayer[i].layer);
	}

    text_layer_init(&dateLayer, GRect(0, 134, SCREENW, TEXTH));
    text_layer_set_background_color(&dateLayer, GColorClear);
    text_layer_set_font(&dateLayer, minuteFont);
    text_layer_set_text_alignment(&dateLayer, GTextAlignmentCenter);
    text_layer_set_text_color(&dateLayer, GColorWhite);
    text_layer_set_text(&dateLayer, date);
    layer_add_child(&bgLayer, &dateLayer.layer);
}

void handle_deinit(AppContextRef ctx) {
    bmp_deinit_container(&hourBmpContainer[0]);
    bmp_deinit_container(&hourBmpContainer[1]);
    fonts_unload_custom_font(minuteFont);
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
