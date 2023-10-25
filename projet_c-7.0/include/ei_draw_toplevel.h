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

#ifndef PROJETC_IG_EI_DRAW_TOPLEVEL_H
#define PROJETC_IG_EI_DRAW_TOPLEVEL_H
void draw_toplevel(ei_surface_t surface,
                   ei_rect_t* clipper,
                   ei_rect_t frame,
                   ei_color_t bg_color,
                   int border_width);

ei_rect_t *ei_intersection(ei_rect_t* my_rect, ei_rect_t* your_rect);
ei_bool_t souris_sur_bande(ei_toplevel* my_toplevel , ei_point_t* where);
ei_bool_t souris_sur_close(ei_toplevel* my_toplevel, ei_point_t* where);
ei_bool_t souris_sur_resizable(ei_toplevel* my_top, ei_point_t* where);
#endif //PROJETC_IG_EI_DRAW_TOPLEVEL_H
