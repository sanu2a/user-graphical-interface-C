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
#include "ei_toplevel.h"
#include "Incallbaccks.h"
#include "ei_placeur.h"

struct cellule_rect {
    ei_rect_t * value;
    struct cellule_rect * next;
};
struct pile{
    struct cellule_rect* head;
    struct cellule_rect * tail;
    int length;

};

struct pile *pile;
bool FINISH = true;
u_int32_t  pick_id_gen = 2;
ei_widget_t * root;
ei_surface_t * root_surface;
ei_surface_t * pick_surface;
ei_rect_t clipper;

ei_rect_t * merge_rect(ei_rect_t * rect_1, ei_rect_t * rect_2){
    ei_point_t  * new_point = malloc(sizeof (ei_point));
    if (rect_1->top_left.x > rect_2->top_left.x){
        new_point->x = rect_2->top_left.x;
    }else{
        new_point->x = rect_1->top_left.x;
    }
    if (rect_1->top_left.y > rect_2->top_left.y){
        new_point->y = rect_2->top_left.y;
    }else{
        new_point->y = rect_1->top_left.y;
    }
    ei_size_t *new_size = malloc(sizeof (ei_size_t));
    if (rect_1->top_left.x > rect_2->top_left.x){
        new_size->width = rect_1->size.width + (rect_1->top_left.x - rect_2->top_left.x);
    }else{
        new_size->width = rect_2->size.width + (rect_2->top_left.x - rect_1->top_left.x);
    }
    if (rect_1->top_left.y > rect_2->top_left.y){
        new_size->height = rect_1->size.height + (rect_1->top_left.y - rect_2->top_left.y);
    }else{
        new_size->height = rect_2->size.height + (rect_2->top_left.y - rect_1->top_left.y) ;
    }
    ei_rect_t * new_rect = malloc(sizeof (ei_rect_t));
    new_rect->size = *new_size;
    new_rect->top_left = *new_point;
    return new_rect;

}


void update(ei_widget_t * widget){
    widget->pick_color = (ei_color_t *) & (widget -> pick_id);
    widget->pick_color->alpha = 0xFF;
}

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen){
    hw_init();

    root_surface = hw_create_window(main_window_size, fullscreen);
    ei_surface_t  root_const = (const ei_surface_t) root_surface;
    pick_surface = hw_surface_create(root_const, main_window_size, false);
    frame_register();
    button_register();
    toplevel_register();
    root = ei_widget_create("frame",NULL, NULL, NULL);
    ei_frame_configure(root, &main_window_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    root->requested_size = hw_surface_get_size(root_surface);
    clipper = (ei_rect_t ){{0,0}, hw_surface_get_size(root_surface)};
}

void ei_app_free(void){
    hw_surface_free(root_surface);
    hw_quit();
}
void ei_app_offscreen_rec(ei_widget_t* widget){
    if (widget == NULL){
        return;
    }
    else{
        widget->pick_id = pick_id_gen;
        update(widget);
        pick_id_gen += 1;
    }
    ei_app_offscreen_rec(widget->children_head);
    ei_app_offscreen_rec(widget->next_sibling);
}
void ei_app_run_rec(ei_widget_t* widget){
    if (widget == NULL){
        return;
    }
    if (widget == root ) {
        widget->pick_id = pick_id_gen;
        update(widget);
        pick_id_gen += 1;
        root->wclass->drawfunc(root, root_surface, pick_surface, NULL);
    }
    else{
        widget->pick_id = pick_id_gen;
        ei_button * my_button = (ei_button *) widget;
        update(widget);
        pick_id_gen += 1;
        widget->wclass->drawfunc(widget, root_surface, pick_surface, &(widget->parent->screen_location));
    }
    ei_app_run_rec(widget->children_head);
    ei_app_run_rec(widget->next_sibling);
}
void redraw_move_reshape(struct pile * pile, ei_widget_t * widget){
    if(pile->head != pile->tail){
        ei_rect_t * widget_delete  = pile->head->value;
        pile->head = pile->head->next;
        ei_rect_t  * widget_create = pile->head->value;
        pile->length = 1;
        ei_rect_t *new_rect = merge_rect(widget_create, widget_delete);
        clipper = *new_rect;
        ei_geometrymanager_placeur_t * placeur = (ei_geometrymanager_placeur_t *) widget->geom_params;
        int new_x = 0;
        int new_y = 0;
        if(widget_create->top_left.x >=widget->screen_location.top_left.x) {
            new_x = placeur->x + (widget_create->top_left.x) - widget->screen_location.top_left.x;
        }else{
            new_x = placeur->x + (widget_create->top_left.x) - widget->screen_location.top_left.x;
        }if(widget_create->top_left.y >=widget->screen_location.top_left.y) {
            new_y = placeur->y + (widget_create->top_left.y) - widget->screen_location.top_left.y;
        }else{
            new_y = placeur->y + (widget_create->top_left.y) - widget->screen_location.top_left.y;
        }

        ei_place(widget, &(placeur->anchor), &(new_x), &(new_y),
                 &(widget_create->size.width), &(widget_create->size.height), (float *) &(placeur->relx), (float *) &(placeur->rely), (float *) &(placeur->relwidth), (float *)&(placeur->relheight));
        hw_surface_lock(root_surface);
        hw_surface_lock(pick_surface);
        ei_app_run_rec((widget->parent));
        hw_surface_unlock(root_surface);
        hw_surface_unlock(pick_surface);
        hw_surface_update_rects(root_surface, NULL);
    }
}
/*void ei_app_run_rec(ei_widget_t* widget){
    if (widget == root ) {
        widget->wclass->drawfunc(widget, root_surface, NULL,clipper);
    }
    ei_widget_t* courant = widget->children_head;
    while( courant!= NULL ){
        ei_app_run_rec()
    }
}*/
void ei_app_run(void){
    hw_surface_lock(root_surface);
    hw_surface_lock(pick_surface);
    //ei_app_offscreen_rec(ei_app_root_widget());
    ei_app_run_rec(ei_app_root_widget());
    hw_surface_unlock(root_surface);
    hw_surface_unlock(pick_surface);
    hw_surface_update_rects(root_surface, NULL);
    while(FINISH){
        ei_event_t * new_event = calloc(1, sizeof(ei_event_t));
        hw_event_wait_next(new_event);
        if (new_event->type == ei_ev_mouse_buttondown || new_event->type == ei_ev_mouse_buttonup){
            ei_widget_t *widget_picking = get_widget_from_color(pick_surface,new_event->param.mouse ,root);
            ei_linked_list * callback_event = get_callback_from_event(new_event->type, widget_picking, callbacks, NULL);
            if (callback_event) {
                ei_bool_t next_callback = callback_event->callback(callback_event->widget, new_event, callback_event->user_param);
                hw_surface_lock(root_surface);
                ei_app_run_rec(ei_app_root_widget());
                hw_surface_unlock(root_surface);
                hw_surface_update_rects(root_surface, NULL);
                while(next_callback == EI_FALSE){
                    if(callback_event){
                        callback_event = get_callback_from_event(new_event->type, widget_picking, callback_event->next, NULL);
                        next_callback = callback_event->callback(callback_event->widget, new_event, callback_event->user_param);
                        hw_surface_lock(root_surface);
                        ei_app_run_rec(ei_app_root_widget());
                        hw_surface_unlock(root_surface);
                        hw_surface_update_rects(root_surface, NULL);
                    }else{
                        break;
                    }
                }
            }
        }else if(new_event->type == ei_ev_mouse_move){
            ei_linked_list * callback_event = get_callback_from_event(new_event->type, NULL, callbacks, "all");
            if (callback_event) {
                callback_event->callback(callback_event->widget, new_event, callback_event->user_param);
                if(pile) {
                    if (pile->length == 2) {
                        redraw_move_reshape(pile, callback_event->widget);
                    }
                }
            }

        }else{
            ei_linked_list * callback_event = get_callback_from_event(new_event->type, NULL, callbacks,NULL);
            if (callback_event) {
                ei_bool_t next_callback = callback_event->callback(callback_event->widget, new_event,
                                                                   callback_event->user_param);
                callback_event->callback(callback_event->widget, new_event,
                                         callback_event->user_param);
                hw_surface_lock(root_surface);
                ei_app_run_rec(ei_app_root_widget());
                hw_surface_unlock(root_surface);
                hw_surface_update_rects(root_surface, NULL);
                while(next_callback == EI_FALSE) {
                    if(callback_event) {
                        callback_event = get_callback_from_event(new_event->type, NULL,
                                                                 (callback_event->next),
                                                                 NULL);
                    }else{
                        break;
                    }
                    if (callback_event) {
                        next_callback = callback_event->callback(callback_event->widget,
                                                                 new_event,
                                                                 callback_event->user_param);
                        hw_surface_lock(root_surface);
                        ei_app_run_rec(ei_app_root_widget());
                        hw_surface_unlock(root_surface);
                    }else{
                        break;
                    }
                }

            }
        }



    }

    getchar();
}
void ei_app_quit_request(void){
    FINISH = false;
    hw_quit();
}
ei_widget_t* ei_app_root_widget(void){
    return root;
}
ei_surface_t ei_app_root_surface(void){
    return root_surface;
}
void ei_app_invalidate_rect(ei_rect_t* rect){
    if(!pile) {
        struct pile *new_pile = malloc(sizeof(struct pile));
        new_pile->head = malloc(sizeof(struct cellule_rect));
        new_pile->tail = malloc(sizeof(struct cellule_rect));
        struct cellule_rect *new_cellule = malloc(sizeof(struct cellule_rect));
        new_cellule->value = malloc(sizeof(ei_rect_t));
        new_cellule->value->top_left = rect->top_left;
        new_cellule->value->size = rect->size;
        new_cellule->next = NULL;
        new_pile->head = new_cellule;
        new_pile->tail = new_cellule;
        new_pile->length = 1;
        pile = new_pile;
    } else {
        struct cellule_rect *new_cellule = malloc(sizeof(struct cellule_rect));
        new_cellule->value = malloc(sizeof (ei_rect_t));
        new_cellule->value->top_left = rect->top_left;
        new_cellule->value->size = rect->size;
        pile->tail->next = new_cellule;
        pile->tail = new_cellule;
        pile->length++;
    }


}
