#include "ei_event_manager.h"
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
#include "ei_event_manager.h"
#include "string.h"


ei_linked_list* get_callback_from_event(ei_eventtype_t eventtype, ei_widget_t * widget, ei_linked_list * list, ei_tag_t tag){
        ei_linked_list * follow = list;
        while(follow != NULL){
                if(tag != NULL && follow->tag !=NULL) {
                        if (follow->eventtype == eventtype &&
                            (follow->widget == widget || strcmp(follow->tag, tag) == 0)) {
                                return follow;
                        }
                }else{
                        if (follow->eventtype == eventtype &&follow->widget == widget ) {
                                return follow;
                        }
                }
                follow = follow->next;
        }
        return NULL;
}


