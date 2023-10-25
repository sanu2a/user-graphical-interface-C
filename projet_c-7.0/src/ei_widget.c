#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_frame.h"
#include <stddef.h>
#include "ei_bouton.h"
#include "Incallbaccks.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_frame.h"
#include "ei_bouton.h"
#include "ei_toplevel.h"
#include <stddef.h>
void			ei_frame_configure		(ei_widget_t*		widget,
                                                               ei_size_t*		requested_size,
                                                               const ei_color_t*	color,
                                                               int*			border_width,
                                                               ei_relief_t*		relief,
                                                               char**			text,
                                                               ei_font_t*		text_font,
                                                               ei_color_t*		text_color,
                                                               ei_anchor_t*		text_anchor,
                                                               ei_surface_t*		img,
                                                               ei_rect_t**		img_rect,
                                                               ei_anchor_t*		img_anchor) {


    ei_frame *maframe = (ei_frame *) widget;
    if (color) {
        maframe->color = *color;
    }
    if (relief) {
        maframe->relief = *relief;
    }
    if (border_width) {
        maframe->frame_border_width = *border_width;
    }
    if (text) {
        if (*text != NULL){
        maframe->text = malloc(strlen(*text)*sizeof (char*));
        strcpy(maframe->text, *text);}
    }
    if (text_font) {
        maframe->text_font = *text_font;
    }
    if (text_color) {
        maframe->text_color = *text_color;
    }
    if (text_anchor) {
        maframe->text_anchor = *text_anchor;
    }
    if (img) {
        maframe->img = *img;
    }
    if (img_rect) {
        maframe->img_rect = malloc(sizeof (ei_rect_t));
        *maframe->img_rect = **img_rect;
    }
    if (img_anchor) {
        maframe->img_anchor = *img_anchor;
    }
    if (requested_size) {
        widget->requested_size = *requested_size;
        widget->screen_location.size = *requested_size;
    }else if(&(widget->requested_size)){

    }else {
                if(maframe->text != NULL){
                        int *width = malloc(sizeof(int));
                        int *height = malloc(sizeof (int));
                        hw_text_compute_size(maframe->text,maframe->text_font,width,height);
                        maframe->widget.requested_size.height = (*height)*3;
                        maframe->widget.requested_size.width = (*width)*2;
                }else if(maframe -> img != NULL){

                        if (maframe -> img_rect != NULL){
                                maframe->widget.requested_size = (*img_rect) ->size;
                        }else {
                                maframe->widget.requested_size = hw_surface_get_size(maframe->img);
                        }
                }
                maframe->widget.screen_location.size = maframe->widget.requested_size ;
        }
}




void			ei_button_configure		(ei_widget_t*		widget,
                                                                ei_size_t*		requested_size,
                                                                const ei_color_t*	color,
                                                                int*			border_width,
                                                                int*			corner_radius,
                                                                ei_relief_t*		relief,
                                                                char**			text,
                                                                ei_font_t*		text_font,
                                                                ei_color_t*		text_color,
                                                                ei_anchor_t*		text_anchor,
                                                                ei_surface_t*		img,
                                                                ei_rect_t**		img_rect,
                                                                ei_anchor_t*		img_anchor,
                                                                ei_callback_t*		callback,
                                                                void**			user_param) {

        ei_button *my_button = (ei_button *) widget;
        ei_bind(ei_ev_mouse_buttondown, widget, NULL, button_change, NULL);
        ei_bind(ei_ev_mouse_buttonup, widget, NULL, button_change, NULL);
        if ((my_button->callback)) {
                ei_unbind(ei_ev_mouse_buttonup, widget, NULL, *(my_button->callback), my_button->user_param);
        }
        if (color) {
                my_button->color = *color;
        }
        if (relief) {
                my_button->relief = *relief;
        }
        if (border_width) {
                my_button->button_border_width = *border_width;
        }
        if (text) {
            if (*text != NULL){
                my_button->text = malloc(strlen(*text)*sizeof (char*));
                strcpy(my_button->text, *text);}
            }
        if (text_font) {
                my_button->text_font = *text_font;
        }
        if (text_color) {
                my_button->text_color = *text_color;
        }
        if (text_anchor) {
                my_button->text_anchor = *text_anchor;
        }
        if (img) {
            ei_size_t size = hw_surface_get_size(*img);
            ((ei_button*) widget ) -> img = hw_surface_create(ei_app_root_surface(),size,EI_FALSE);
            ei_copy_surface(((ei_button*) widget ) -> img,NULL,*img,NULL,EI_FALSE );
        }
        if (img_rect) {
            my_button->img_rect = malloc(sizeof (ei_rect_t));
            *((ei_button *)widget) -> img_rect = **img_rect;
        }
        if (img_anchor) {
                my_button->img_anchor = *img_anchor;
        }
        if (user_param) {
                my_button->user_param = *user_param;
        }
        if (callback) {
            my_button->callback = callback;
            if(!user_param){
                ei_bind(ei_ev_mouse_buttonup, widget, NULL, *callback, NULL);
            }else{
                ei_bind(ei_ev_mouse_buttonup, widget, NULL, *callback, *user_param);
            }
        }if(corner_radius){
                my_button->corner_radius = *corner_radius;
        }
    if (requested_size) {
        widget->requested_size = *requested_size;
        widget->screen_location.size = *requested_size;


    }else {
        if(my_button->text != NULL){
            int *width = malloc(sizeof(int));
            int *height = malloc(sizeof (int));
            hw_text_compute_size(my_button->text,my_button->text_font,width,height);
            my_button->widget.requested_size.height = (*height)*3;
            my_button->widget.requested_size.width = (*width)*2;
        }else if(my_button -> img != NULL){

            if (my_button -> img_rect != NULL){
                my_button->widget.requested_size = (*img_rect) ->size;
            }else {
                my_button->widget.requested_size = hw_surface_get_size(my_button->img);
            }
        }
        my_button->widget.screen_location.size = my_button->widget.requested_size ;
    }
}

void			ei_toplevel_configure		(ei_widget_t*		widget,
                                                                  ei_size_t*		requested_size,
                                                                  ei_color_t*		color,
                                                                  int*			border_width,
                                                                  char**			title,
                                                                  ei_bool_t*		closable,
                                                                  ei_axis_set_t*		resizable,
                                                                  ei_size_t**		min_size){
        ei_toplevel* matoplevel = (ei_toplevel*) widget;
        if (requested_size) {
                widget->requested_size = *requested_size;
                widget->screen_location.size.width = requested_size->width ;
                widget->screen_location.size.height = requested_size->height +40;

        }
        if (color){
                matoplevel->color = *color;
        }
        if (border_width){
                matoplevel->border_width = *border_width;
        }
        if (title){
                matoplevel->title = *title;
        }
        if (closable){
                matoplevel->closable = *closable;
        }
        if (resizable){
                matoplevel->resizable = *resizable;
        }
        if (min_size){
                matoplevel->min_size = *min_size;
        }
        ei_bind(ei_ev_mouse_buttondown,widget, NULL, move_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup,widget, NULL, move_toplevel, NULL);
}

ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
                                                             ei_widget_t*		parent,
                                                             void*			user_data,
                                                             ei_widget_destructor_t destructor){
        ei_widgetclass_t * widgetclass = ei_widgetclass_from_name(class_name);
        ei_widget_t * widgetcree = widgetclass->allocfunc();
        widgetcree->children_tail = NULL;
        widgetcree->children_head = NULL;
        widgetcree->next_sibling = NULL;
        widgetcree->wclass = widgetclass;
        widgetcree->parent = parent;
        widgetcree->destructor = destructor;
        widgetclass->setdefaultsfunc(widgetcree);
        if (parent != NULL){
                if (parent->children_head == NULL){
                        parent->children_head = widgetcree;
                        parent->children_tail = widgetcree;
                }
                else {
                        // cas  lorsque on arrive a la fin.Feuille!
                        if (parent->children_head == parent->children_tail) {
                                parent->children_tail = widgetcree;
                                parent->children_head->next_sibling = widgetcree;
                        }
                        else{
                                parent->children_tail->next_sibling = widgetcree;
                                parent->children_tail = widgetcree;
                        }
                }
        }
        return widgetcree;
}
void			ei_widget_destroy		(ei_widget_t*		widget){
    if (widget -> children_head == NULL || widget-> next_sibling == NULL) {
       /*widget->wclass->releasefunc(widget);
       free(widget->pick_color);
       free(widget->content_rect);
       widget->geom_params->manager->releasefunc(widget);*/
    } else {

    }

}
/*typedef struct ei_widget_t {
    ei_widgetclass_t*	wclass;		///< The class of widget of this widget. Avoids the field name "class" which is a keyword in C++.
    uint32_t		pick_id;	///< Id of this widget in the picking offscreen.
    ei_color_t*		pick_color;	///< pick_id encoded as a color.
    void*			user_data;	///< Pointer provided by the programmer for private use. May be NULL.
    ei_widget_destructor_t	destructor;	///< Pointer to the programmer's function to call before destroying this widget. May be NULL.


    struct ei_widget_t*	parent;		///< Pointer to the parent of this widget.
    struct ei_widget_t*	children_head;	///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
    struct ei_widget_t*	children_tail;	///< Pointer to the last child of this widget.
    struct ei_widget_t*	next_sibling;	///< Pointer to the next child of this widget's parent widget.


    struct ei_geometry_param_t*
            geom_params;	///< Pointer to the geometry management parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
    ei_size_t		requested_size;	///< Size requested by the widget (big enough for its label, for example), or by the programmer. This can be different than its screen size defined by the placer.
    ei_rect_t		screen_location;///< Position and size of the widget expressed in the root window reference.
    ei_rect_t*		content_rect;	///< Where to place children, when this widget is used as a container. By defaults, points to the screen_location.
} ei_widget_t;*/
