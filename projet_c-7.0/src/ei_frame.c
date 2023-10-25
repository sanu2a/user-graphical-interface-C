#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_frame.h"
#include "ei_draw.h"
#include "ei_draw_toplevel.h"


ei_point_t  point_draw(ei_size_t size_obj,ei_rect_t screen_location, ei_anchor_t  anchor){
       int text_width = size_obj.width;
       int text_height = size_obj.height;
        ei_point_t  point ;
        int x_widget = screen_location.top_left.x;
        int y_widget = screen_location.top_left.y;
        int width_widget = screen_location.size.width;
        int height_widget = screen_location.size.height;
        if( anchor == ei_anc_center ){
                int width = x_widget+ width_widget/2 - text_width/2;
                int height = y_widget + height_widget/2 - text_height/2;
                point.x = width;
                point.y = height;
        }
        if(anchor == ei_anc_east){
                int width = x_widget + width_widget  - text_width;
                int height = y_widget + height_widget/2 - text_height/2 ;
                point.x = width;
                point.y = height;
        }
        if(anchor == ei_anc_north){
                int width = x_widget + width_widget/2 - text_width/2;
                int height =  y_widget  ;
                point.x = width;
                point.y = height;
        }
        if(anchor == ei_anc_south){
                int width = x_widget + width_widget/2 - text_width/2;
                int height = y_widget + height_widget  - text_height;
                point.x = width;
                point.y = height;
        }
        if(anchor == ei_anc_west){
                int width = x_widget  ;
                int height = y_widget + height_widget/2 - text_height/2;
                point.x = width;
                point.y = height;
        }
        if(anchor == ei_anc_northeast){
                int width = x_widget + width_widget - text_width;
                int height = y_widget ;
                point.x = width;
                point.y = height;
        }
        if(anchor == ei_anc_southeast){
                int width = x_widget + width_widget - text_width  ;
                int height =y_widget + height_widget - text_height ;
                point.x = width;
                point.y = height;
        }
        if(anchor == ei_anc_northwest){
            int width = x_widget ;
            int height = y_widget ;
            point.x = width;
            point.y = height;
        }
        if(anchor == ei_anc_southwest){
                int width = x_widget  ;
                int height = y_widget + height_widget  - text_height;
                point.x = width;
                point.y = height;
        }
        return  point;
}


ei_widget_t * frame_allocfunc(void){
        return calloc(1, sizeof(ei_frame));
}
void release_frame(ei_widget_t *widget){
        ei_frame *maframe = (ei_frame*) widget;
        if (maframe->text_font){
                hw_text_font_free(maframe->text_font);
        }
        if (maframe->text){
                free(maframe->text);
        }
        if (maframe->img){
                hw_surface_free(maframe->img);
        }

}
void frame_drawfunc(struct ei_widget_t* widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper){
        // creation des linked points
        ei_frame *maframe = (ei_frame*)  widget;
        ei_draw_button(surface, clipper,widget->screen_location, maframe->color,maframe->relief,maframe->frame_border_width, 0);
        ei_draw_button(pick_surface,clipper,widget->screen_location, *(maframe->widget.pick_color), ei_relief_none, 0,0);
    if (maframe->text){
        ei_size_t size_text;
        hw_text_compute_size(maframe->text,maframe->text_font,&(size_text.width),&(size_text.height));
       ei_point_t where = point_draw(size_text, widget->screen_location,maframe->text_anchor);
       ei_draw_text(surface,&where,(const char *)maframe->text,maframe->text_font,maframe->text_color, clipper);
    }
        else if (maframe->img){
                ei_rect_t img_rect = hw_surface_get_rect((const ei_surface_t )maframe->img);
                ei_point_t  where = point_draw(img_rect.size,widget->screen_location,maframe->img_anchor);
                ei_rect_t draw_rect = {where.x, where.y};
                draw_rect.size.width = widget->screen_location.size.width - where.x + widget->screen_location.top_left.x;
                draw_rect.size.height = widget->screen_location.size.height - where.y + widget->screen_location.top_left.y;
                if (maframe->img_rect){
                        ei_rect_t *new_rect = ei_intersection(&widget->screen_location, maframe->img_rect);
                        if (new_rect) {
                                draw_rect.size.width = new_rect->size.width;
                                draw_rect.size.height = new_rect->size.height;
                                draw_rect.top_left = new_rect->top_left;
                        }
                }
                if (widget->parent->screen_location.size.width < where.x + draw_rect.size.width +widget->screen_location.top_left.x|| widget->parent->screen_location.size.height < where.y + draw_rect.size.height  +widget->screen_location.top_left.y){
                        draw_rect.size.width = widget->parent->screen_location.size.width  - where.x ;
                        draw_rect.size.height = widget->parent->screen_location.size.height - where.y;
                }
                img_rect.size.width = draw_rect.size.width;
                img_rect.size.height = draw_rect.size.height;
                ei_copy_surface(surface,&draw_rect,maframe->img, &img_rect,EI_FALSE);
        }
}
void setdefaultframe(ei_widget_t* widget){
        ei_frame * maframe = (ei_frame*) widget;
        maframe->relief = ei_relief_none;
        maframe->font = ei_default_font;
        maframe->color = ei_default_background_color ;
        maframe->widget.requested_size = (ei_size_t){0,0} ;
        maframe->frame_border_width = 0;
        maframe->text = NULL;
        maframe->text_font = ei_default_font;
        maframe->text_anchor = ei_anc_center;
        maframe->text_color = ei_font_default_color;
        maframe->img = NULL;
        maframe->img_rect = NULL;
        maframe->img_anchor = ei_anc_center;
}

ei_widgetclass_t  maframe =(ei_widgetclass_t) {"frame",&frame_allocfunc,&release_frame,&frame_drawfunc,&setdefaultframe};
void frame_register(){
        ei_widgetclass_register(&maframe);}



