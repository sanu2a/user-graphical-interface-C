#include <stdlib.h>
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include "ei_types.h"
#include "math.h"

#ifndef PROJETC_IG_OFFSCREEN_H
#define PROJETC_IG_OFFSCREEN_H


ei_color_t * pick(ei_surface_t surfa, ei_point_t* where);
ei_widget_t * ei_widget_rec(ei_color_t* color_id,ei_widget_t* widget);
ei_widget_t * get_widget_from_color(ei_surface_t surface_offscreen,ei_mouse_event_t mouse_event,ei_widget_t* widget_root);

#endif //PROJETC_IG_OFFSCREEN_H
