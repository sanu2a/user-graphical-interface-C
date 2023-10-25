//
// Created by killuamam on 13/05/22.
//
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

#ifndef PROJETC_IG_EI_DRAW_BUTTON_H
#define PROJETC_IG_EI_DRAW_BUTTON_H



ei_linked_point_t * point_arc(ei_point_t centre, int rayon, float angle_debut, float angle_fin);
ei_linked_point_t* ajouter_liste_chainee(ei_linked_point_t *liste1, ei_linked_point_t *liste2, ei_linked_point_t* liste3);
ei_linked_point_t ** rounded_frame(ei_rect_t frame, float radius);
void free_linked_point(ei_linked_point_t* first_point) ;
void ei_draw_button(ei_surface_t surface,
                    const ei_rect_t* clipper,
                    ei_rect_t frame,
                    ei_color_t color,
                    ei_relief_t relief,
                    int epaisseur_bande,
                    float rayon);
ei_rect_t *ei_intersection(ei_rect_t* my_rect, ei_rect_t* your_rect);
#endif //PROJETC_IG_EI_DRAW_BUTTON_H
