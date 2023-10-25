//
// Created by killuamam on 13/05/22.
//
#include <stdlib.h>
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include "ei_types.h"
#include "ei_toplevel.h"
#include "ei_draw_toplevel.h"
#include "ei_draw_button.h"

ei_widget_t * top_level_alloc()
{
        return calloc(1, sizeof(ei_toplevel));
}
void setdefault_toplevel(ei_widget_t * widget){
        ei_toplevel* matoplevel = (ei_toplevel*) widget;
        ei_size_t default_size = ei_size(320,420);
        matoplevel->requested_size = &default_size;
        matoplevel->color = ei_font_default_color;
        matoplevel->border_width = 4;
        char * default_title = "Toplevel";
        matoplevel->title = default_title;
        matoplevel->closable = EI_TRUE;
        matoplevel->resizable = ei_axis_both;
        matoplevel->min_size = malloc(sizeof (ei_size_t));
        matoplevel->min_size->height = 120;
        matoplevel->min_size->width =  160;
}
void toplevel_drawfunc( ei_widget_t* widget,
                        ei_surface_t surface,
                        ei_surface_t pick_surface,
                        ei_rect_t* clipper){
        ei_toplevel * top = (ei_toplevel *)widget;
        draw_toplevel(surface,clipper,widget->screen_location,((ei_toplevel*) widget)->color,((ei_toplevel*) widget)->border_width);
        ei_rect_t * inter = ei_intersection(&widget->screen_location,clipper);
        ei_rect_t text_clip = *clipper;
        if(inter){

                ei_fill(pick_surface,widget->pick_color,inter);
                text_clip = *inter;

        }
        ei_color_t text_color = {255,255,255,255};
        ei_point_t text_where = {widget->screen_location.top_left.x + top->border_width + 35,widget->screen_location.top_left.y + 5};
        ei_rect_t *clipp = ei_intersection(clipper, &ei_app_root_widget()->screen_location);
        ei_draw_text(surface, &text_where, top->title,NULL, text_color, clipper);
        if (top->closable) {
                ei_point_t center = {widget->screen_location.top_left.x + top->border_width + 15, widget->screen_location.top_left.y+ 20};
                ei_linked_point_t *cercle = point_arc(center, 10, 0, 2 * M_PI);
                ei_color_t color_close = {255, 0, 0, 255};
                ei_draw_polygon(surface, cercle, color_close, &text_clip);
        }
        if (top -> resizable){
                ei_rect_t *rect_size = malloc(sizeof(ei_rect_t));
                rect_size->size.width = widget ->screen_location.size.width*0.05 ;
                rect_size->size.height = widget ->screen_location.size.height*0.05;
                rect_size->top_left.x = widget->screen_location.top_left.x+widget->screen_location.size.width-rect_size->size.width +1;
                rect_size->top_left.y =  widget->screen_location.top_left.y -rect_size->size.height + widget->screen_location.size.height + 1 ;
                ei_color_t *color = malloc(sizeof(ei_color_t));
                color->red = 0.5 * top->color.red;
                color->green = 0.5 * top->color.green;
                color->blue = 0.5 * top->color.blue;
                color->alpha = top->color.alpha;
                ei_rect_t *rect_size1 = ei_intersection(rect_size,clipper);
                if(rect_size1){
                        ei_fill(surface, color, rect_size1);
                }
                free(rect_size1);
                free(rect_size);
        }
        free(inter);

}
void release_toplevel(ei_widget_t *widget){
        ei_toplevel* top = (ei_toplevel*) widget;
        if (top->title){
                free(top->title);
        }
        if (top -> requested_size){
                free(top -> requested_size);
        }
        if (top -> min_size){
                free(top -> min_size);
        }

}

ei_widgetclass_t  toplevel =(ei_widgetclass_t) {"toplevel",&top_level_alloc,&release_toplevel,&toplevel_drawfunc,&setdefault_toplevel};
void toplevel_register(){
        ei_widgetclass_register(&toplevel);
}