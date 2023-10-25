#include "ei_application.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_event.h"
#include "ei_types.h"
#include "stdbool.h"
#include "ei_bouton.h"
#include "ei_event_manager.h"
#include "offscreen.h"
#include "ei_event.h"
#include "Incallbaccks.h"
#include "ei_draw_toplevel.h"
#include "ei_toplevel.h"



ei_rect_t * new_rect;
int x_gap;
int y_gap;

ei_bool_t button_change(ei_widget_t* widget, ei_event_t* event, void* user_param){
    ei_button* button = (ei_button *) widget;
    if (event->type == ei_ev_mouse_buttondown){
        button->previous_relief = button->relief;
        button->relief = ei_relief_sunken;
        if(event->type == ei_ev_mouse_buttonup){
            return EI_TRUE;
        }
    }if (event->type == ei_ev_mouse_buttonup){
        button->relief = button->previous_relief;
        return EI_FALSE;
    }
}
ei_bool_t move_widget_pointer(ei_widget_t* widget, ei_event_t* event, void* user_param){
    if(event->type == ei_ev_mouse_move){
        new_rect->top_left.x = event->param.mouse.where.x - x_gap;
        new_rect->top_left.y = event->param.mouse.where.y - y_gap;
        ei_app_invalidate_rect(new_rect);
    }
}

ei_bool_t resize_widget_pointer(ei_widget_t* widget, ei_event_t* event, void* user_param){
    if(event->type == ei_ev_mouse_move){
        ei_toplevel * my_toplevel = (ei_toplevel *) widget;
        if(my_toplevel->resizable == ei_axis_both || my_toplevel->resizable == ei_axis_x){
            new_rect->size.width = event->param.mouse.where.x - new_rect->top_left.x + new_rect->size.width*0.02 ;
        }if(my_toplevel->resizable == ei_axis_both || my_toplevel->resizable == ei_axis_y){
            new_rect->size.height = event->param.mouse.where.y - new_rect->top_left.y + new_rect->size.height*0.02;
        }
        if(new_rect->size.height >= my_toplevel->min_size->height && new_rect->size.width >= my_toplevel->min_size->width ) {
            ei_app_invalidate_rect(new_rect);
        }else{
            if(event->type = ei_ev_mouse_buttonup){
                ei_unbind(ei_ev_mouse_move, widget, "all", resize_widget_pointer, NULL);
            }
        }
    }
}

ei_bool_t move_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param){
    ei_toplevel * my_toplevel = (ei_toplevel * ) widget;
    ei_point_t  remember_me = event->param.mouse.where;
    x_gap = remember_me.x - widget->screen_location.top_left.x;
    y_gap = remember_me.y - widget->screen_location.top_left.y;

    if(souris_sur_resizable(my_toplevel , &(event->param.mouse.where))) {
        if (event->type == ei_ev_mouse_buttondown) {
            if (!new_rect) {
                ei_rect_t *rect_int = malloc((sizeof(rect_int)));
                rect_int->top_left.x = widget->screen_location.top_left.x;
                rect_int->top_left.y = widget->screen_location.top_left.y;
                rect_int->size = widget->screen_location.size;
                new_rect = rect_int;
            } else {
                new_rect->top_left = widget->screen_location.top_left;
            }
            if(my_toplevel->resizable != ei_axis_none) {
                ei_bind(ei_ev_mouse_move, widget, "all", resize_widget_pointer, NULL);
            }
        }
        if (event->type == ei_ev_mouse_buttonup) {
            if(my_toplevel->resizable != ei_axis_none){
                ei_unbind(ei_ev_mouse_move, widget, "all", resize_widget_pointer, NULL);
            }
        }
    }else if(souris_sur_bande(my_toplevel , &(event->param.mouse.where))) {
        if (event->type == ei_ev_mouse_buttondown) {
            if (!new_rect) {
                ei_rect_t *rect_int = malloc((sizeof(rect_int)));
                rect_int->top_left.x = 0;
                rect_int->top_left.y = 0;
                rect_int->size = widget->screen_location.size;
                new_rect = rect_int;
            } else {
                new_rect->size = widget->screen_location.size;
            }
            ei_bind(ei_ev_mouse_move, widget, "all", move_widget_pointer, NULL);
        }
        if (event->type == ei_ev_mouse_buttonup) {
            ei_unbind(ei_ev_mouse_move, widget, "all", move_widget_pointer, NULL);
        }
    }else if(souris_sur_close(my_toplevel , &(event->param.mouse.where))){
        ei_app_quit_request();
    }
    return EI_TRUE;

}




