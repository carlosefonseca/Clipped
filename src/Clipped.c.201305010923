#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xB4, 0x6D, 0x25, 0x42, 0x6F, 0x7D, 0x46, 0x2C, 0x87, 0x9B, 0xE2, 0x1A, 0xE0, 0xDD, 0x69, 0xEF }
PBL_APP_INFO(MY_UUID,
             "Clipped", "Jnm",
             1, 4, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define CX 72
#define CY 84
#define SCREENW 144
#define SCREENH 168
#define TEXTX 1
#define TEXTY 96
#define TEXTW 142
#define TEXTH 60

#define LANG_DUTCH 0
#define LANG_ENGLISH 1
#define LANG_FRENCH 2
#define LANG_GERMAN 3
#define LANG_SPANISH 4
#define LANG_MAX 5

#define BIGMINUTES true
#define USDATE false
#define WEEKDAY true
#define SMALLDIGITS_WHITE false
#define LANG_CUR LANG_ENGLISH

const int digitImage[10] = {
    RESOURCE_ID_IMAGE_D0, RESOURCE_ID_IMAGE_D1, RESOURCE_ID_IMAGE_D2, RESOURCE_ID_IMAGE_D3,
    RESOURCE_ID_IMAGE_D4, RESOURCE_ID_IMAGE_D5, RESOURCE_ID_IMAGE_D6, RESOURCE_ID_IMAGE_D7,
    RESOURCE_ID_IMAGE_D8, RESOURCE_ID_IMAGE_D9
};


const char weekDay[LANG_MAX][7][3] = {
	{ "zon", "maa", "din", "woe", "don", "vri", "zat" },	// Dutch
	{ "sun", "mon", "tue", "wed", "thu", "fri", "sat" },	// English
	{ "dim", "lun", "mar", "mer", "jeu", "ven", "sam" },	// French
	{ "son", "mon", "die", "mit", "don", "fre", "sam" },	// German
	{ "dom", "lun", "mar", "mie", "jue", "vie", "sab" }		// Spanish
};

typedef struct {
    Layer layer;
    int pos;
    BmpContainer bmpContainer;
    GRect frame;
    int curDigit;
    int prevDigit;
} bigDigit;

Window window;
Layer bgLayer;
bigDigit bigSlot[2];
TextLayer smallDigitLayer[5], dateLayer;
GFont customFont;
char smallDigits[] = "00";
char date[] = "000000";
int h1, h2, m1, m2, D1, D2, M1, M2, wd;
bool clock12 = false;
int dx[5] = { -2, 2, 2, -2, 0 };
int dy[5] = { -2, -2, 2, 2, 0 };
GColor textColor[5] = { GColorWhite, GColorWhite, GColorWhite, GColorWhite, GColorBlack };
PblTm now, last = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };

void updateLayer(Layer *layer, GContext* ctx) {
    int i, width = 8;

    for (i=0; i<2; i++) {
        if (bigSlot[i].curDigit != bigSlot[i].prevDigit) {
            layer_remove_from_parent(&bigSlot[i].bmpContainer.layer.layer);
            bmp_deinit_container(&bigSlot[i].bmpContainer);
            bmp_init_container(digitImage[bigSlot[i].curDigit], &bigSlot[i].bmpContainer);
            bigSlot[i].frame = layer_get_frame(&bigSlot[i].bmpContainer.layer.layer);
        }
        width += bigSlot[i].frame.size.w;
    }

    bigSlot[0].frame.origin.x = CX - width/2;
    bigSlot[1].frame.origin.x = bigSlot[0].frame.origin.x + bigSlot[0].frame.size.w + 4;

    for (i=0; i<2; i++) {
        layer_set_frame(&bigSlot[i].layer, bigSlot[i].frame);
        if (bigSlot[i].curDigit != bigSlot[i].prevDigit) {
            layer_add_child(&bigSlot[i].layer, &bigSlot[i].bmpContainer.layer.layer);
        }
    }
}

void setHM(PblTm *tm) {
	int h, i;
	
	if (tm->tm_hour != last.tm_hour) {
		h= tm->tm_hour;
        if (clock12) {
            h = h%12;
			if (h == 0) {
				h = 12;
			}
		}
        bigSlot[0].prevDigit = bigSlot[0].curDigit;
        bigSlot[1].prevDigit = bigSlot[1].curDigit;
        h1 = h/10;
        h2 = h%10;
		if (BIGMINUTES) {
    		smallDigits[0] = '0' + (char)h1;
    		smallDigits[1] = '0' + (char)h2;
            for (i=0; i<5; i++) {
                text_layer_set_text(&smallDigitLayer[i], smallDigits);
            }
		} else {
            bigSlot[0].curDigit = h1;
            bigSlot[1].curDigit = h2;
            layer_mark_dirty(&bgLayer);
        }
	}

	if (tm->tm_min != last.tm_min) {
    	m1 = tm->tm_min/10;
    	m2 = tm->tm_min%10;
		if (BIGMINUTES) {
            bigSlot[0].curDigit = m1;
            bigSlot[1].curDigit = m2;
            layer_mark_dirty(&bgLayer);
        } else {
    		smallDigits[0] = '0' + (char)m1;
    		smallDigits[1] = '0' + (char)m2;
            for (i=0; i<5; i++) {
                text_layer_set_text(&smallDigitLayer[i], smallDigits);
            }
		}
	}

	if (tm->tm_mday != last.tm_mday) {
    	D1 = tm->tm_mday/10;
    	D2 = tm->tm_mday%10;

		if (tm->tm_mon != last.tm_mon) {
            M1 = (tm->tm_mon+1)/10;
            M2 = (tm->tm_mon+1)%10;
        }
        
        if (tm->tm_wday != last.tm_wday) {
            wd = tm->tm_wday;
        }

        if (WEEKDAY) {
                date[0] = weekDay[LANG_CUR][wd][0];
                date[1] = weekDay[LANG_CUR][wd][1];
                date[2] = weekDay[LANG_CUR][wd][2];
                date[3] = ' ';
                date[4] = '0' + (char)D1;
                date[5] = '0' + (char)D2;
        } else {
            if (USDATE) {
                date[0] = '0' + (char)M1;
                date[1] = '0' + (char)M2;
                date[2] = ' ';
                date[3] = '0' + (char)D1;
                date[4] = '0' + (char)D2;
            } else {
                date[0] = '0' + (char)D1;
                date[1] = '0' + (char)D2;
                date[2] = ' ';
                date[3] = '0' + (char)M1;
                date[4] = '0' + (char)M2;
            }
            date[5] = (char)0;
        }
        text_layer_set_text(&dateLayer, date);
    }

	last = now;
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *evt) {
	setHM(evt->tick_time);
}

void handle_init(AppContextRef ctx) {
    int i;
   
    window_init(&window, "Clipped");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorBlack);
    
    resource_init_current_app(&APP_RESOURCES);

    customFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BORIS_37));
    
	clock12 = !clock_is_24h_style();

    layer_init(&bgLayer, GRect(0, 0, SCREENW, 138));
    layer_set_update_proc(&bgLayer, &updateLayer);
    layer_add_child(&window.layer, &bgLayer);
    
    for (i=0; i<2; i++) {
        bigSlot[i].pos = i;
        bigSlot[i].curDigit = -1;
        layer_init(&bigSlot[i].layer, GRect(72*i, 0, 72, 138));
        layer_add_child(&bgLayer, &bigSlot[i].layer);
    }
    
	for (i=0; i<5; i++) {
    	text_layer_init(&smallDigitLayer[i], GRect(TEXTX+dx[i], TEXTY+dy[i], TEXTW, TEXTH));
    	text_layer_set_background_color(&smallDigitLayer[i], GColorClear);
    	text_layer_set_font(&smallDigitLayer[i], customFont);
		if (BIGMINUTES) {
    		text_layer_set_text_alignment(&smallDigitLayer[i], GTextAlignmentLeft);
		} else {
    		text_layer_set_text_alignment(&smallDigitLayer[i], GTextAlignmentRight);
		}
		if (SMALLDIGITS_WHITE) {
			if (textColor[i] == GColorBlack) {
				textColor[i] = GColorWhite;
			} else {
				textColor[i] = GColorBlack;
			}
		}
    	text_layer_set_text_color(&smallDigitLayer[i], textColor[i]);
    	text_layer_set_text(&smallDigitLayer[i], smallDigits);
    	layer_add_child(&window.layer, &smallDigitLayer[i].layer);
	}

    text_layer_init(&dateLayer, GRect(-20, 134, SCREENW+40, TEXTH));
    text_layer_set_background_color(&dateLayer, GColorClear);
    text_layer_set_font(&dateLayer, customFont);
    text_layer_set_text_alignment(&dateLayer, GTextAlignmentCenter);
    text_layer_set_text_color(&dateLayer, GColorWhite);
    text_layer_set_text(&dateLayer, date);
    layer_add_child(&window.layer, &dateLayer.layer);

    get_time(&now);
	setHM(&now);
}

void handle_deinit(AppContextRef ctx) {
    bmp_deinit_container(&bigSlot[0].bmpContainer);
    bmp_deinit_container(&bigSlot[1].bmpContainer);
    fonts_unload_custom_font(customFont);
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
