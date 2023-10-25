//
// Created by killuamam on 13/05/22.
//
#include "ei_geometrymanager.h"
#include "ei_types.h"
#include "ei_draw.h"
#include "ei_event.h"
#include "hw_interface.h"
#ifndef PROJETC_IG_EI_TOPLEVEL_H
#define PROJETC_IG_EI_TOPLEVEL_H

typedef struct ei_toplevel{
        ei_widget_t		widget;
        ei_size_t*		requested_size;
        ei_color_t		color;
        int			border_width;
        char*			title;
        ei_bool_t		closable;
        ei_axis_set_t		resizable;
        ei_size_t*	min_size;
}ei_toplevel;
ei_widget_t * top_level_alloc();
void setdefault_toplevel(ei_widget_t * widget);
void toplevel_drawfunc(struct ei_widget_t* widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t* clipper);
void toplevel_register();

#endif //PROJETC_IG_EI_TOPLEVEL_H
