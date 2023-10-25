#include <stdlib.h>
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include "ei_types.h"
#include "math.h"

ei_color_t * pick(ei_surface_t surfa, ei_point_t* where) {
    ei_size_t surface = hw_surface_get_size(surfa);
    int ir;
    int ig;
    int ib;
    int ia;
    uint32_t position_pixel_picking = surface.width * where->y + where->x;
    hw_surface_lock(surfa);
    uint8_t *adresse = hw_surface_get_buffer(surfa);
    hw_surface_get_channel_indices(surfa, &ir, &ig, &ib, &ia);
    adresse =  adresse + position_pixel_picking * 4;
    ei_color_t *my_color = malloc(sizeof(ei_color_t));
    my_color -> red = adresse[ir];
    my_color -> green = adresse[ig];
    my_color -> blue = adresse[ib];
    my_color -> alpha =  255;
    hw_surface_unlock(surfa);
    return my_color;
}
ei_widget_t  * widget_return = NULL;

ei_widget_t * ei_widget_rec(ei_color_t* color_id,ei_widget_t* widget){
        if (widget == NULL){
                return widget_return;
        }
        if ( widget->pick_color->red == color_id ->red && widget->pick_color->blue == color_id->blue && widget->pick_color->green == color_id->green && widget->pick_color->alpha == color_id->alpha){
                widget_return = widget;
                return widget_return;
        }
        ei_widget_rec(color_id,widget->children_head);
        ei_widget_rec(color_id,widget->next_sibling);
}
ei_widget_t * get_widget_from_color(ei_surface_t surface_offscreen,ei_mouse_event_t mouse_event,ei_widget_t* widget_root)
{
        ei_point_t position = mouse_event.where;
        ei_color_t * color_id = pick(surface_offscreen, &position);
        ei_widget_t * widget_pick= ei_widget_rec(color_id,widget_root);
        return widget_pick;
}


