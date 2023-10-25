
#ifndef PROJETC_IG_INCALLBACCKS_H
#define PROJETC_IG_INCALLBACCKS_H

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

ei_bool_t button_change(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t button_return(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t move_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param);


#endif //PROJETC_IG_INCALLBACCKS_H
