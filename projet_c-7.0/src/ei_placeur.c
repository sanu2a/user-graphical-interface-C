#include "ei_geometrymanager.h"
#include "ei_types.h"

#include "ei_placeur.h"

void	ei_geometrymanager_placeur_runfunc_t(struct ei_widget_t*	widget) {
    ei_geometrymanager_placeur_t *geom_params_placer = (ei_geometrymanager_placeur_t *) widget->geom_params;
    int width = geom_params_placer->width;
    int height = geom_params_placer->height;
    int x = geom_params_placer->x;
    int y = geom_params_placer->y;
    float rel_width = geom_params_placer->relwidth;
    float rel_height = geom_params_placer->relheight;
    float rel_x = geom_params_placer->relx;
    float rel_y = geom_params_placer->rely;
    ei_anchor_t anchor = geom_params_placer->anchor;
    if (widget->parent) {
        int width_parent = widget->parent->screen_location.size.width;
        int height_parent = widget->parent->screen_location.size.height;
        int x_parent = widget->parent->screen_location.top_left.x;
        int y_parent = widget->parent->screen_location.top_left.y;
        widget->screen_location.size.width = width ;
        widget->screen_location.size.height = height;
        int ajout = 0;
        if((strcmp((widget->parent->wclass->name),"toplevel") ==0)){
            ajout = 40;
        }

        if(anchor == ei_anc_southeast) {
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent - width;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent - height - ajout;

        }

        if(anchor == ei_anc_northeast) {
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent - width;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent + ajout;
        }

        if(anchor == ei_anc_center) {
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent - width / 2;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent - height / 2 + ajout;
        }
        if(anchor==ei_anc_west) {
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent - height / 2 + ajout;
        }
        if(anchor ==  ei_anc_east) {
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent - width;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent - height / 2 + ajout;
        }
        if(anchor == ei_anc_southwest) {
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent - height - ajout;
        }
        if (anchor == ei_anc_south) {
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent - width / 2;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent - height - ajout;
        }
        if( anchor == ei_anc_north) {
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent - width / 2;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent + ajout;
        }
        if(anchor == ei_anc_northwest){
            widget->screen_location.top_left.x = x + rel_x * width_parent + x_parent;
            widget->screen_location.top_left.y = y + rel_y * height_parent + y_parent+ajout;
        }
    } else {
        widget->screen_location.size.width = (rel_width) ? (int) (width * rel_width) : width;
        widget->screen_location.size.height = (rel_height) ? (int) (height * rel_height) : height;
        widget->screen_location.top_left.x = (rel_x) ? (int) (width * rel_x) : x;
        widget->screen_location.top_left.y = (rel_y) ? (int) (width * rel_y) : y;
    }
    ei_widget_t * children = widget->children_head;

    while(children && children->geom_params){
        ei_geometrymanager_placeur_t *placeur_children= (ei_geometrymanager_placeur_t *) children->geom_params;
        float rel_width = placeur_children->relwidth;
        float rel_height = placeur_children->relheight;
        int new_width = rel_width * widget->screen_location.size.width;
        int new_height = rel_height * widget->screen_location.size.height;
        int new_x = (placeur_children->x)*new_width/(children->screen_location.size.width) ;
        int new_y = (placeur_children->y)*new_height/(children->screen_location.size.height);
        float rel_x = placeur_children->relx;
        float rel_y = placeur_children->rely;
        ei_place(children, &(placeur_children->anchor), &new_x, &new_y, &new_width, &new_height, &(rel_x), &(rel_y),&(placeur_children->relwidth) , &(placeur_children->relheight));
        children = children->next_sibling;
    }

}


void ei_geometrymanager_placeur_releasefunc_t(struct ei_widget_t*	widget){

}


//
// Created by killuamam on 07/05/22.
//