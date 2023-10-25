#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

#ifndef PROJETC_IG_EI_FRAME_H
#define PROJETC_IG_EI_FRAME_H
typedef struct ei_frame {
    ei_widget_t widget;
    ei_size_t size;
    ei_color_t color;
    ei_relief_t relief;
    int	frame_border_width;
    ei_font_t font;
    char* text;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_font_t  text_font;
    ei_surface_t  img;
    ei_rect_t*  img_rect;
    ei_anchor_t img_anchor;
} ei_frame;
void frame_drawfunc(struct ei_widget_t* widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper);

void setdefaultframe(ei_widget_t* widget);
void release_frame(ei_widget_t* widget);
ei_widget_t * frame_allocfunc();
void frame_register();
ei_point_t  point_draw(ei_size_t size_obj,ei_rect_t screen_location, ei_anchor_t  anchor);

#endif //PROJETC_IG_EI_FRAME_H
