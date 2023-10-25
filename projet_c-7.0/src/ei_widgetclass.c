#include "ei_widgetclass.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

ei_widgetclass_t * widgetclasses = NULL;

void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass){
    widgetclass->next = widgetclasses;
    widgetclasses = widgetclass;
}
ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name){
    ei_widgetclass_t *widgetcourant = widgetclasses;
    while (widgetcourant != NULL){
        if (strcmp(widgetcourant->name,name) == 0 ){
            return widgetcourant;
        }
        else{
            widgetcourant = widgetcourant->next;
        }
    }
    return NULL;
}
