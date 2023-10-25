#include <stdlib.h>
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include "ei_types.h"
#include "math.h"
#include "ei_bouton.h"
#include "math.h"
#include "ei_frame.h"
#include "ei_draw_toplevel.h"

ei_widget_t * button_allocfunc(void){
    return calloc(1, sizeof (ei_button));
}

void setdefaultbutton(ei_widget_t* widget) {
    ei_button *my_button = (ei_button *) widget;
    my_button->relief = ei_relief_none;
    my_button->font = ei_default_font;
    my_button->color = ei_default_background_color;
    my_button->size = (ei_size_t) {0, 0};
    my_button->button_border_width = k_default_button_border_width;
    my_button->text = NULL;
    my_button->text_font = ei_default_font;
    my_button->text_anchor = ei_anc_center;
    my_button->text_color = ei_font_default_color;
    my_button->img = NULL;
    my_button->img_rect = NULL;
    my_button->img_anchor = ei_anc_center;
    my_button->corner_radius = k_default_button_corner_radius;
    my_button->user_param = NULL;
    my_button->callback = NULL;
}

void release_button(ei_widget_t* widget) {
    ei_button *my_butona = (ei_button *) widget;
    if (my_butona->text) {
        free(my_butona->text);
    }
    if (my_butona -> callback){
        free(my_butona -> callback);
    }
    if ( my_butona -> user_param){
        free(my_butona -> user_param);
    }
    if(my_butona -> img_rect){
        free(my_butona -> img_rect);
    }
    if (my_butona-> img != NULL) {
        hw_surface_free(*(my_butona->img));
    }
    free(my_butona);
}
void button_drawfunc(ei_widget_t* widget, ei_surface_t surface,
                     ei_surface_t offscreen, ei_rect_t *clipper) {
    ei_button *my_button = (ei_button *) widget;
    ei_draw_button(surface, clipper, widget->screen_location, my_button->color, my_button->relief,
                   my_button->button_border_width, my_button->corner_radius);
    ei_draw_button(offscreen, clipper, widget->screen_location, *(widget->pick_color), ei_relief_none,
                   my_button->button_border_width, my_button->corner_radius);
    if (my_button->text) {
        ei_size_t size_text;
        hw_text_compute_size(my_button->text, my_button->text_font, &(size_text.width), &(size_text.height));
        ei_point_t where = point_draw(size_text, widget->screen_location, my_button->text_anchor);
        ei_rect_t *new_clipper = ei_intersection(clipper,&ei_app_root_widget()->screen_location);
        ei_draw_text(surface, &where, my_button->text, my_button->text_font, my_button->text_color, new_clipper);
    }
    if (my_button->img) {
        ei_rect_t draw_rect;
        ei_anchor_t img_anchor = ei_anc_northwest;
        ei_size_t img_size = hw_surface_get_size(my_button->img);
        ei_point_t where = point_draw(img_size, widget->screen_location, img_anchor);
        draw_rect.top_left = ei_point(where.x + 2,where.y + 2);
        if (my_button->img_rect == NULL){
            draw_rect.size = hw_surface_get_rect(my_button->img).size;
        }
        else {
            draw_rect.size = my_button->img_rect->size;
        }
        ei_rect_t src_rect = {{0,0},draw_rect.size};
        if(my_button->img_rect != NULL) {
            src_rect.top_left = my_button->img_rect->top_left;
        }
        src_rect.size = draw_rect.size;
        ei_copy_surface(surface,&draw_rect,my_button->img,&src_rect,EI_FALSE);
    }
}




ei_widgetclass_t  my_button =(ei_widgetclass_t) {"button",&button_allocfunc,&release_button,&button_drawfunc,&setdefaultbutton};

void button_register(void){
    ei_widgetclass_register(&my_button);
}