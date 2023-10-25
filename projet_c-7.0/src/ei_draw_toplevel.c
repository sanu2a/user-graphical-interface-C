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
#include "ei_draw_button.h"



ei_linked_point_t * rounded_toplevel(ei_rect_t frame) {
        /* arc de gauche haut */
        ei_point_t center1 = {frame.top_left.x + 10, frame.top_left.y + 10};
        ei_linked_point_t *arc_gauche_haut = point_arc(center1, 10, M_PI / 2, M_PI);
        ei_linked_point_t *point_gauche_bas = malloc(sizeof(ei_linked_point_t));
        point_gauche_bas->point.x = frame.top_left.x;
        point_gauche_bas->point.y = frame.top_left.y + frame.size.height;
        point_gauche_bas->next = NULL;
        ei_linked_point_t *point_droite_bas = malloc(sizeof(ei_linked_point_t));
        point_droite_bas->point.x = frame.top_left.x + frame.size.width;
        point_droite_bas->point.y = frame.top_left.y + frame.size.height;
        point_droite_bas->next = NULL;
        ei_linked_point_t *partie1 = ajouter_liste_chainee(arc_gauche_haut, point_gauche_bas, point_droite_bas);
        /* arc de droite haut */
        ei_point_t center2 = {frame.top_left.x + frame.size.width - 10, frame.top_left.y + 10};
        ei_linked_point_t *arc_droite_haut = point_arc(center2, 10, 0, M_PI / 2);
        /* toplevel */
        ei_linked_point_t *toplevel = ajouter_liste_chainee(partie1, arc_droite_haut, NULL);
        return toplevel;
}
void draw_toplevel(ei_surface_t surface,
                   ei_rect_t* clipper,
                   ei_rect_t frame,
                   ei_color_t bg_color,
                   int border_width) {
        ei_linked_point_t *toplevel1 = rounded_toplevel(frame);
        ei_color_t *color = malloc(sizeof(ei_color_t));
        color->red = 0.8 * bg_color.red;
        color->green = 0.8 * bg_color.green;
        color->blue = 0.8 * bg_color.blue;
        color->alpha = bg_color.alpha;
        ei_draw_polygon(surface, toplevel1, bg_color, clipper);
        free_linked_point(toplevel1);
        int border_height = 40;
        ei_rect_t *clipper_toplevel = malloc(sizeof(ei_rect_t));
        clipper_toplevel->top_left.x = frame.top_left.x + border_width;
        clipper_toplevel->top_left.y = frame.top_left.y + border_height;
        clipper_toplevel->size.width = frame.size.width - 2 * border_width;
        clipper_toplevel->size.height = frame.size.height - border_width - border_height;
        ei_rect_t * clip = ei_intersection(clipper,clipper_toplevel);
        if(clip){
                ei_fill(surface, color, clip);
        }
        free(clipper_toplevel);
}

ei_bool_t souris_sur_close(ei_toplevel* my_toplevel, ei_point_t* where){
        ei_widget_t *widget = (ei_widget_t *) my_toplevel;
        ei_point_t centre = {widget->screen_location.top_left.x + my_toplevel->border_width + 15, widget->screen_location.top_left.y+ 20};
        float distance = sqrt(powf(where ->x - centre.x,2) + powf(where->y - centre.y,2));
        float  cl = distance;
        return (distance < 10);
}

ei_bool_t souris_sur_bande(ei_toplevel* my_toplevel , ei_point_t* where){
        ei_widget_t *widget = (ei_widget_t *) my_toplevel;
        ei_point_t center = {widget->screen_location.top_left.x + my_toplevel->border_width + 15 + 10, widget->screen_location.top_left.y};
        if(widget->screen_location.top_left.x < where->x
           && where->x < widget->screen_location.top_left.x + widget->screen_location.size.width &&
           widget->screen_location.top_left.y < where->y && where->y < widget->screen_location.top_left.y + 40 ){
                return (souris_sur_close(my_toplevel,where) == EI_FALSE);
        }
        return EI_FALSE;
}


ei_bool_t souris_sur_resizable(ei_toplevel* my_top, ei_point_t* where){
        ei_widget_t *widget = (ei_widget_t *) my_top;
        ei_rect_t *rect_size = malloc(sizeof (ei_rect_t));
        rect_size->size.width = widget ->screen_location.size.width*0.05 ;
        rect_size->size.height = widget ->screen_location.size.height*0.05;
        rect_size->top_left.x = widget->screen_location.top_left.x+widget->screen_location.size.width-rect_size->size.width +1;
        rect_size->top_left.y =  widget->screen_location.top_left.y -rect_size->size.height + widget->screen_location.size.height + 1 ;
        if(rect_size -> top_left.x < where-> x && where->x < rect_size ->top_left.x + rect_size->size.width && rect_size -> top_left.y < where-> y && where ->y < rect_size ->top_left.y + rect_size->size.height){
                return EI_TRUE;
        }
        return EI_FALSE;
}







