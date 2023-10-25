#include "ei_geometrymanager.h"
#include "ei_types.h"
#include "ei_draw.h"
#include "ei_event.h"
#include "hw_interface.h"

#ifndef PROJECT_IG_EI_PLACEUR_H
#define PROJECT_IG_EI_PLACEUR_H


typedef struct ei_geometrymanager_placeur_t{
        ei_geometry_param_t *placeur;
        int x;
        int y;
        int relx;
        int rely;
        int width;
        int height;
        float relwidth;
        float relheight;
        ei_anchor_t anchor;
} ei_geometrymanager_placeur_t;

char	ei_geometrymanager_name_placeur_t[20];

void	ei_geometrymanager_placeur_runfunc_t(struct ei_widget_t*	widget);

void	ei_geometrymanager_placeur_releasefunc_t(struct ei_widget_t*	widget);

#endif //PROJECT_IG_EI_PLACEUR_H
