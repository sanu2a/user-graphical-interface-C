
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_placeur.h"
#include "ei_geometrymanager.h"
ei_geometrymanager_t *  geometrymanagers = NULL;

void			ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager){
    if(geometrymanagers){
        geometrymanager->next = geometrymanagers;
        geometrymanagers = geometrymanager; }
    geometrymanagers = geometrymanager;
}
ei_geometrymanager_t * ei_geometrymanager_from_name(ei_geometrymanager_name_t name){
    ei_geometrymanager_t *courant = geometrymanagers;
    while (courant){
        if(strcmp(courant ->name,name) ==0 ){
            return courant;
        }
        courant = courant -> next;
    }
    return NULL;
}
ei_geometrymanager_t geometrymanager_placeur = (ei_geometrymanager_t) {"placeur", &ei_geometrymanager_placeur_runfunc_t,&ei_geometrymanager_placeur_releasefunc_t,NULL};
void 			ei_register_placer_manager 	(void){
    ei_geometrymanager_register(&geometrymanager_placeur);
}

void			ei_place			(ei_widget_t*		widget,
                                     ei_anchor_t*		anchor,
                                     int*			x,
                                     int*			y,
                                     int*			width,
                                     int*			height,
                                     float*			rel_x,
                                     float*			rel_y,
                                     float*			rel_width,
                                     float*			rel_height) {
    if (!(widget->geom_params)){
        widget->geom_params = (ei_geometry_param_t*) calloc(1, sizeof(ei_geometrymanager_placeur_t));
        widget -> geom_params -> manager = (ei_geometrymanager_t*)ei_geometrymanager_from_name("placeur");
    }
    ei_geometrymanager_placeur_t * geom_params_placeur = (ei_geometrymanager_placeur_t *)widget->geom_params;
    if (width){
        geom_params_placeur->width = *width;
    }
    if (height){
        geom_params_placeur->height = *height;
    }
    if (rel_width){
        geom_params_placeur->relwidth = *rel_width;
    }
    if (rel_height){
        geom_params_placeur->relheight = *rel_height;
    }
    if (anchor){
        geom_params_placeur->anchor = *anchor;
    }
    if (!anchor){
        geom_params_placeur->anchor = ei_anc_northwest;
    }
    if (!(width) && !(rel_width)){
        geom_params_placeur->width = widget->screen_location.size.width;
        geom_params_placeur->relwidth = (float)(widget->screen_location.size.width) /(widget->parent->screen_location.size.width);
    }
    if (!(height) && !(rel_height)) {
        geom_params_placeur->height = widget->screen_location.size.height;
        geom_params_placeur->relheight = (float)(widget->screen_location.size.height) /(widget->parent->screen_location.size.height);
    }
    if (!width && rel_width){
        geom_params_placeur->width = (*rel_width)*widget->parent->screen_location.size.width;
    }
    if (!height && rel_height){
        geom_params_placeur->height = (*rel_width)*widget->parent->screen_location.size.width;
    }
    if (x){
        geom_params_placeur->x = *x;
    }
    if (y){
        geom_params_placeur->y = *y;
    }
    if (rel_x){
        geom_params_placeur->relx = *rel_x;
    }
    if (rel_y){
        geom_params_placeur->rely = *rel_y;
    }
    widget->geom_params = (ei_geometry_param_t *) geom_params_placeur;
    ei_geometrymanager_placeur_runfunc_t(widget);
}
//

void ei_placeur_releasefunc_t( ei_widget_t*	widget)
{
    ei_widget_t * fils = widget -> children_head;
    while (fils != NULL){
        fils = fils -> next_sibling;
    }
}
void ei_placeur_unmap(ei_widget_t * widget)
{
    if(!(widget->geom_params)){
        return;
    } else {
        ei_placeur_releasefunc_t(widget);
        free(widget->geom_params);
    }
}
