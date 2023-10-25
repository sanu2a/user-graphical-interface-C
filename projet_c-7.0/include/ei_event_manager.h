//
// Created by killuamam on 09/05/22.
//
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

#ifndef PROJETC_IG_EI_EVENT_MANAGER_H


typedef struct ei_linked_list {
        ei_eventtype_t eventtype;
        ei_callback_t callback;
        ei_widget_t *widget;
        ei_tag_t tag;
        void *user_param;
        struct ei_linked_list* next;
} ei_linked_list;
ei_linked_list * get_callback_from_event(ei_eventtype_t eventtype, ei_widget_t * widget, ei_linked_list * list, ei_tag_t tag);

ei_linked_list * callbacks;
#define PROJETC_IG_EI_EVENT_MANAGER_H

#endif //PROJETC_IG_EI_EVENT_MANAGER_H
