#ifndef PROJETC_IG_EI_BOUTON_H
#define PROJETC_IG_EI_BOUTON_H

#include <stdlib.h>
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include "ei_types.h"
#include "math.h"


typedef struct ei_button {
        ei_widget_t widget;
        ei_size_t size;
        ei_color_t color;
        ei_relief_t relief;
        ei_relief_t previous_relief;
        int	button_border_width;
        int corner_radius;
        ei_font_t font;
        char* text;
        ei_color_t text_color;
        ei_anchor_t text_anchor;
        ei_font_t  text_font;
        ei_surface_t*  img;
        ei_rect_t*  img_rect;
        ei_callback_t*		callback;
        ei_anchor_t img_anchor;
        ei_linked_tag_t tag;
        void * user_param;
} ei_button;
void button_drawfunc(struct ei_widget_t* widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper);

void setdefaultbutton(ei_widget_t* widget);
void release_button(ei_widget_t* widget);
ei_widget_t * button_allocfunc(void);
void button_register(void);
void ei_draw_button(ei_surface_t surface,
                    const ei_rect_t* clipper,
                    ei_rect_t frame,
                    ei_color_t color,
                    ei_relief_t relief,
                    int epaisseur_bande,
                    float rayon);
#endif //PROJETC_IG_EI_BOUTON_H
