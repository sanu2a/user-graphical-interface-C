#include <stdlib.h>
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include "ei_types.h"
#include "math.h"
#include "ei_draw.h"
# define M_PI           3.14159265358979323846  /* pi */

ei_rect_t *ei_intersection(ei_rect_t* my_rect, ei_rect_t* your_rect) {
        if(your_rect != NULL && my_rect !=NULL) {
                int cordx1 = fmax(my_rect->top_left.x, your_rect->top_left.x);
                int cordx2 = fmin(my_rect->top_left.x + my_rect->size.width,
                                  your_rect->top_left.x + your_rect->size.width);
                int cordy1 = fmax(my_rect->top_left.y, your_rect->top_left.y);
                int cordy2 = fmin(my_rect->top_left.y + my_rect->size.height,
                                  your_rect->top_left.y + your_rect->size.height);
                if (cordx2 <= cordx1 || cordy2 <= cordy1)
                        return NULL;

                ei_rect_t *rec_inter = malloc(sizeof(ei_rect_t));
                rec_inter->top_left.x = cordx1;
                rec_inter->top_left.y = cordy1;
                rec_inter->size.height = cordy2 - cordy1;
                rec_inter->size.width = cordx2 - cordx1;
                return rec_inter;
        }
        return NULL;
}

ei_linked_point_t * point_arc(ei_point_t centre, int rayon, float angle_debut, float angle_fin)
{
        float pas=  (angle_fin- angle_debut) /rayon;
        /* variable tete_liste pour sauver la tete de la liste chainée*/
        ei_linked_point_t *tete_liste = malloc(sizeof(ei_linked_point_t ));
        tete_liste -> point.x = centre.x + rayon * cos(angle_debut);
        tete_liste -> point.y = centre.y - rayon * sin(angle_debut);
        tete_liste -> next = NULL;
        /* variable point_courant pour chainer les points*/
        ei_linked_point_t *point_courant = tete_liste;
        for(float angle = (angle_debut + pas);angle < angle_fin; angle += pas) {
                ei_linked_point_t* my_point = malloc(sizeof(ei_linked_point_t));
                my_point -> point.x = centre.x + rayon * cos(angle);
                my_point -> point.y = centre.y - rayon * sin(angle);
                my_point -> next = NULL;
                point_courant -> next = my_point;
                point_courant = point_courant -> next  ;
        }
        return tete_liste;
}

/* lier deux liste chainées (tete1 -> queue1 -> tete2 -> queue2)*/
ei_linked_point_t* ajouter_liste_chainee(ei_linked_point_t *liste1, ei_linked_point_t *liste2, ei_linked_point_t* liste3)
{
        ei_linked_point_t* point_courant = liste1;
        while (point_courant -> next  != NULL){
                point_courant = point_courant -> next;
        }
        point_courant -> next = liste2;
        if(liste3){
                ei_linked_point_t* point_courant_1 = liste2;
                while (point_courant_1 -> next  != NULL){
                        point_courant_1 = point_courant_1 -> next;
                }
                point_courant_1 -> next = liste3;
        }
        return liste1;
}




ei_linked_point_t ** rounded_frame(ei_rect_t frame, float radius)
{
        int parametre_H = (frame.size.height < frame.size.width) ? frame.size.height/2 : frame.size.width/2;
        ei_point_t center = {frame.top_left.x + radius, frame.top_left.y + radius};
        ei_linked_point_t* haut_gauche = point_arc(center, radius, M_PI / 2, M_PI);
        ei_linked_point_t* haut_gauche2 = point_arc(center, radius, M_PI / 2, M_PI);
        center.y += frame.size.height - 2 * radius;
        ei_linked_point_t* bas_gauche = point_arc( center, radius, M_PI, 3 * M_PI / 2);
        center.x += frame.size.width - 2 * radius;
        ei_linked_point_t* bas_droite = point_arc(center, radius, 3 * M_PI / 2, 2 * M_PI);
        ei_linked_point_t* bas_droite2 = point_arc(center, radius, 3 * M_PI / 2, 2 * M_PI);
        center.y = frame.top_left.y + radius;
        ei_linked_point_t* haut_droite = point_arc(center, radius, 0, M_PI / 2);
        /* rectangle point arrondis */
        ei_linked_point_t *l = ajouter_liste_chainee(haut_gauche ,bas_gauche,bas_droite);
        ei_linked_point_t *lm = ajouter_liste_chainee(l,haut_droite, NULL);
        /* demi arc gauche bas*/
        ei_point_t center1 = {frame.top_left.x + radius, frame.top_left.y +frame.size.height - radius};
        ei_linked_point_t* bas_h_gauche_semi= point_arc(center1, radius, M_PI, 5*M_PI /4);
        ei_linked_point_t* point = malloc(sizeof(ei_linked_point_t));
        point->point.x = frame.top_left.x + parametre_H;
        point->point.y = frame.top_left.y + frame.size.height - parametre_H;
        point -> next = NULL;
        ei_linked_point_t* next_point = malloc(sizeof(ei_linked_point_t));
        next_point->point.x = frame.top_left.x + frame.size.width - parametre_H;
        next_point->point.y = point -> point.y;
        next_point -> next = NULL;
        ei_linked_point_t * arc_1 = ajouter_liste_chainee(bas_h_gauche_semi,point,next_point);
        /*demi arc bas droite*/
        ei_point_t center2 = {frame.top_left.x + frame.size.width - radius, frame.top_left.y + radius};
        ei_linked_point_t* haut_gauche_semi= point_arc(center2, radius, M_PI/4, M_PI /2);
        ei_linked_point_t * arc_2 = ajouter_liste_chainee(arc_1,haut_gauche_semi,NULL);
        /*partie haut*/
        ei_linked_point_t * arc_3 = ajouter_liste_chainee(arc_2,haut_gauche2,NULL);
        /*partie bas*/
        ei_linked_point_t* haut_bas_drt_semi= point_arc(center2, radius, 0, M_PI /4);
        ei_linked_point_t* point1 = malloc(sizeof(ei_linked_point_t));
        point1->point.x = frame.top_left.x + frame.size.width - parametre_H;
        point1->point.y = frame.top_left.y + frame.size.height - parametre_H;
        point1 -> next = NULL;
        ei_linked_point_t* next_point1 = malloc(sizeof(ei_linked_point_t));
        next_point1 -> point.x = frame.top_left.x +parametre_H;
        next_point1 -> point.y = point1->point.y;
        next_point1 -> next = NULL;
        ei_linked_point_t * arc_4 = ajouter_liste_chainee(haut_bas_drt_semi,point1,next_point1);
        /*demi arc bas gauche*/
        ei_point_t center3 = {frame.top_left.x +radius, frame.top_left.y+ frame.size.height- radius};
        ei_linked_point_t* bas_gauche_semi= point_arc(center3, radius, 5*M_PI/4, 3*M_PI /2);
        /* partie bas*/
        ei_linked_point_t * arc_5 = ajouter_liste_chainee(arc_4,bas_gauche_semi,bas_droite2);
        /* tuple qui contient partie haute et bas et rec tout entier*/
        ei_linked_point_t ** my_tuple = malloc(3*sizeof(ei_linked_point_t));
        my_tuple[0] = lm;
        my_tuple[1] = arc_3;
        my_tuple[2] = arc_5;
        return my_tuple;

}

/* free les liste chainee*/
void free_linked_point(ei_linked_point_t* first_point) {
        ei_linked_point_t* next_point;
        while (first_point != NULL) {
                next_point = first_point->next;
                free(first_point);
                first_point = next_point;
        }
}
/* dessine button relief*/
void ei_draw_button(ei_surface_t surface,
                    const ei_rect_t* clipper,
                    ei_rect_t frame,
                    ei_color_t color,
                    ei_relief_t relief,
                    int epaisseur_bande,
                    float rayon){
        /* créer tuple de points de la partie haute et les points de tout le rectangle*/
        ei_linked_point_t **my_tuple = rounded_frame(frame,rayon);
        ei_rect_t * frame_cli = ei_intersection(&frame,(ei_rect_t *)clipper);
        if (relief != ei_relief_none && epaisseur_bande) {
                float factor = 0.8;
                ei_color_t normal = {color.red * (1 - factor),
                                     color.green * (1 - factor),
                                     color.blue * (1 - factor),
                                     color.alpha};
                float factor1 = 3;
                ei_color_t enfonce = {color.red + (255 - color.red) * factor1,
                                      color.green + (255 - color.green) * factor1,
                                      color.blue + (255 - color.blue) * factor1,
                                      color.alpha};
                /* dessiner le button en fonction de degré de relief*/
                if (relief == ei_relief_sunken) {
                        ei_draw_polygon(surface, (const ei_linked_point_t *) my_tuple[2],enfonce, frame_cli);
                        ei_draw_polygon(surface, (const ei_linked_point_t *) my_tuple[1], normal, frame_cli);
                } else {
                        ei_draw_polygon(surface, (const ei_linked_point_t *) my_tuple[2], normal, frame_cli);
                        ei_draw_polygon(surface, (const ei_linked_point_t *) my_tuple[1], enfonce, frame_cli);

                }

                free_linked_point(my_tuple[1]);
                free_linked_point(my_tuple[0]);
                free_linked_point(my_tuple[2]);
                /* dessiner fond de button */
                frame.size.height -= 2 * epaisseur_bande;
                frame.size.width -= 2 * epaisseur_bande;
                frame.top_left.x += epaisseur_bande;
                frame.top_left.y += epaisseur_bande;
                ei_linked_point_t **point_fond = rounded_frame(frame, rayon);
                color.alpha = 255;
                ei_draw_polygon(surface, point_fond[0], color, frame_cli);
                free_linked_point(point_fond[0]);
                free_linked_point(point_fond[1]);
                free_linked_point(point_fond[2]);
        } else if (rayon) {
            frame.size.height += 2 * epaisseur_bande;
            frame.size.width += 2 * epaisseur_bande;
            frame.top_left.x -= epaisseur_bande;
            frame.top_left.y -= epaisseur_bande;
            ei_linked_point_t **point_fond1 = rounded_frame(frame, rayon);
            ei_draw_polygon(surface, point_fond1[0], color, frame_cli);
            free_linked_point(point_fond1[0]);
            free_linked_point(point_fond1[1]);
            free_linked_point(point_fond1[2]);


        }else{
            ei_fill(surface, &color, frame_cli);
        }
}