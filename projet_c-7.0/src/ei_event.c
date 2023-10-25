#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_event_manager.h"


// creation d'une liste chainee

/**
 * \brief	Binds a callback to an event type and a widget or a tag.
 *
 * @param	eventtype	The type of the event.
 * @param	widget		The callback is only called if the event is related to this widget.
 *				This parameter must be NULL if the "tag" parameter is not NULL.
 * @param	tag		The callback is only called if the event is related to a widget that
 *				has this tag. A tag can be a widget class name, or the tag "all".
 *				This parameter must be NULL is the "widget" parameter is not NULL.
 * @param	callback	The callback (i.e. the function to call).
 * @param	user_param	A user parameter that will be passed to the callback when it is called.
 */
void		ei_bind			(ei_eventtype_t		 eventtype,
                                                    ei_widget_t*		widget,
                                                    ei_tag_t		tag,
                                                    ei_callback_t		callback,
                                                    void*			user_param){
        ei_linked_list *binding = calloc(1, sizeof(ei_linked_list)) ;
        binding->eventtype = eventtype;
        binding->widget = widget;
        binding->tag = tag;
        binding->callback = callback;
        binding->user_param = user_param;
        binding->next = NULL;
        if (callbacks == NULL){
                callbacks = binding;
        }else{
                ei_linked_list *courant = callbacks;
                while (courant->next != NULL){
                        courant = courant -> next;
                }
                courant->next = binding;
        }
}
/**
 * \brief	Unbinds a callback from an event type and widget or tag.
 *
 * @param	eventtype, widget, tag, callback, user_param
 *				All parameters must have the same value as when \ref ei_bind was
 *				called to create the binding.
 */
void		ei_unbind		(ei_eventtype_t		eventtype,
                                              ei_widget_t*		widget,
                                              ei_tag_t		tag,
                                              ei_callback_t		callback,
                                              void*			user_param) {

        ei_linked_list sentinelle = (ei_linked_list) {ei_ev_none,NULL,NULL,NULL,NULL,callbacks };
        ei_linked_list *parcours = &sentinelle;

        while (parcours->next != NULL && parcours->next->widget != widget || parcours->next->eventtype != eventtype || parcours->next->user_param != user_param || parcours->next->callback != callback || parcours->next->tag != tag ) {
                parcours = parcours->next;
        }

        if (parcours->next != NULL) {

                ei_linked_list *unbinding = parcours->next;
                parcours->next = unbinding->next;
                free(unbinding);
        }
        callbacks = sentinelle.next;
}