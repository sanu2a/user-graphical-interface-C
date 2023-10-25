#include "ei_draw.h"
#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"


#include "ei_draw.h"
#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw_toplevel.h"

/**
 * \brief	Copies pixels from a source surface to a destination surface.
 *		The source and destination areas of the copy (either the entire surfaces, or
 *		subparts) must have the same size before considering clipping.
 *		Both surfaces must be *locked* by \ref hw_surface_lock.
 *
 * @param	destination	The surface on which to copy pixels.
 * @param	dst_rect	If NULL, the entire destination surface is used. If not NULL,
 *				defines the rectangle on the destination surface where to copy
 *				the pixels.
 * @param	source		The surface from which to copy pixels.
 * @param	src_rect	If NULL, the entire source surface is used. If not NULL, defines the
 *				rectangle on the source surface from which to copy the pixels.
 * @param	alpha		If true, the final pixels are a combination of source and
 *				destination pixels weighted by the source alpha channel and
 *				the transparency of the final pixels is set to opaque.
 *				If false, the final pixels are an exact copy of the source pixels,
 				including the alpha channel.
 *
 * @return			Returns 0 on success, 1 on failure (different sizes between source and destination).
 */




int			ei_copy_surface		(ei_surface_t		destination,
                                       const ei_rect_t*	dst_rect,
                                       ei_surface_t		source,
                                       const ei_rect_t*	src_rect,
                                       ei_bool_t		alpha) {
    // adresse point de depart de source
    int *ir_src = malloc(sizeof(int));
    int *ir_dst = malloc(sizeof(int));
    int *ig_src = malloc(sizeof(int));
    int *ig_dst = malloc(sizeof(int));
    int *ib_src = malloc(sizeof(int));
    int *ib_dst = malloc(sizeof(int));
    int *ia_src = malloc(sizeof(int));
    int *ia_dst = malloc(sizeof(int));
    ei_rect_t dst_rect_new;
    ei_rect_t src_rect_new;
    // getting the size of surface_src and surface_dst
    ei_rect_t src_rect_surf = hw_surface_get_rect(source);
    ei_rect_t dst_rect_surf = hw_surface_get_rect(destination);
    // cas des NULL we use the entire surface for the source and destination
    if (src_rect == NULL) {
        src_rect_new = src_rect_surf;
    } else {
        src_rect_new = *src_rect;
    }
    if (dst_rect == NULL) {
        dst_rect_new = dst_rect_surf;
    } else {
        dst_rect_new = *dst_rect;
    }
    /*|| (src_rect_surf.size.width != dst_rect_surf.size.width) || (src_rect_surf.size.height != dst_rect_surf.size.height)*/
    // in case we dont have the same size
    if ((src_rect_new.size.height != dst_rect_new.size.height) ||
        (src_rect_new.size.width != dst_rect_new.size.width)) {
        return 1;
    }
    // recueprer adress of the (0,0) pixel in both surface_src and surface_dst
    uint32_t *start_sr = (uint32_t *) hw_surface_get_buffer(source);
    uint32_t *start_ds = (uint32_t *) hw_surface_get_buffer(destination);
    hw_surface_get_channel_indices(source, ir_src, ig_src, ib_src, ia_src);
    hw_surface_get_channel_indices(destination, ir_dst, ig_dst, ib_dst, ia_dst);
    for (int line = 0; line < src_rect_new.size.height; line++) {
        for (int col = 0; col < src_rect_new.size.width; col++) {
            uint32_t *cour_pixel_sc = start_sr + (src_rect_new.top_left.y + line) * src_rect_surf.size.width + src_rect_new.top_left.x + col;
            uint32_t *cour_pixel_ds = start_ds + (dst_rect_new.top_left.y + line) * dst_rect_surf.size.width + dst_rect_new.top_left.x + col;
            if (alpha == EI_FALSE) {
                *cour_pixel_ds = *cour_pixel_sc;
            }
            else{
                uint8_t tab_sc[4];
                uint8_t tab_ds[4];
                tab_sc[3] = *cour_pixel_sc >> 24;
                tab_sc[2] = *cour_pixel_sc >> 16;
                tab_sc[1] = *cour_pixel_sc >>  8;
                tab_sc[0] = *cour_pixel_sc;
                tab_ds[3] = *cour_pixel_ds >> 24;
                tab_ds[2] = *cour_pixel_ds >> 16;
                tab_ds[1] = *cour_pixel_ds >>  8;
                tab_ds[0] = *cour_pixel_ds ;
                uint8_t tab_ds_cop[4];
                tab_ds_cop[0] = tab_ds[0];
                tab_ds_cop[1] = tab_ds[1];
                tab_ds_cop[2] = tab_ds[2];
                tab_ds_cop[3] = tab_ds[3];
                tab_ds[*ir_dst] = (tab_sc[*ir_src]*tab_sc[*ia_src]+(255-tab_sc[*ia_src])*tab_ds_cop[*ir_dst])/255;
                tab_ds[*ig_dst] = (tab_sc[*ig_src]*tab_sc[*ia_src]+(255-tab_sc[*ia_src])*tab_ds_cop[*ig_dst])/255;
                tab_ds[*ib_dst] = (tab_sc[*ib_src]*tab_sc[*ia_src]+(255-tab_sc[*ia_src])*tab_ds_cop[*ib_dst])/255;
                tab_ds[*ia_dst] = 255;
                *cour_pixel_ds = tab_ds[0] | (tab_ds[1] << 8) | (tab_ds[2] << 16) | (tab_ds[3] << 24);
            }
        }
    }
    free(ir_src);
    free(ig_src);
    free(ib_src);
    free(ia_src);
    free(ir_dst);
    free(ig_dst);
    free(ib_dst);
    free(ia_dst);

    return 0;
}


void			ei_draw_text		(ei_surface_t		surface,
                                     const ei_point_t*	where,
                                     const char*		text,
                                     ei_font_t		font,
                                     ei_color_t		color,
                                     const ei_rect_t*	clipper){
    // clipper non null intersection de deux rectangles
    if (!text ){
        return;
    }if (text[0] == '\0'){
        return;
    }
    if(!font) {
        font = ei_default_font;
    }
    ei_surface_t surf_txt = hw_text_create_surface(text,font,color);
    ei_rect_t rect_text = hw_surface_get_rect(surf_txt);
    ei_rect_t draw_rect = {where->x, where->y, rect_text.size};
    if (clipper){
        ei_rect_t * new_rect = ei_intersection(clipper,&draw_rect);
        if (new_rect ){
            if(new_rect->top_left.x > draw_rect.top_left.x) rect_text.top_left.x += new_rect->top_left.x - draw_rect.top_left.x ;
            draw_rect.top_left = new_rect->top_left;
            draw_rect.size.width = new_rect->size.width;
            draw_rect.size.height = new_rect->size.height;
            rect_text.size.width = new_rect->size.width;
            rect_text.size.height = new_rect->size.height;
        }
    }
    hw_surface_lock(surf_txt);
    ei_copy_surface(surface,&draw_rect ,surf_txt,&rect_text,EI_TRUE);
    hw_surface_unlock(surf_txt);
}
void			ei_fill			(ei_surface_t		surface,
                                    const ei_color_t*	color,
                                    const ei_rect_t*	clipper) {
    uint32_t *depart = (uint32_t *) hw_surface_get_buffer(surface);
    int *ir_change = malloc(sizeof(int));
    int *ig_change = malloc(sizeof(int));
    int *ib_change = malloc(sizeof(int));
    int *ia_change = malloc(sizeof(int));
    ei_size_t surface_size = hw_surface_get_size(surface);
    ei_rect_t src_rect_new = hw_surface_get_rect(surface);
    hw_surface_get_channel_indices(surface, ir_change, ig_change, ib_change, ia_change);
    int i = ((clipper)? clipper->top_left.x : src_rect_new.top_left.x);
    int j = ((clipper)? clipper->top_left.y : src_rect_new.top_left.y);
    int i_fin = ((clipper)?clipper->top_left.x + clipper->size.width : surface_size.width);
    int j_fin = ((clipper)? clipper->top_left.y + clipper->size.height : surface_size.height );
    for (int line = j ; line < j_fin; line++) {
        for (int col = i ; col < i_fin; col++) {
            uint32_t *cour_pixel_sc =
                    (depart + (src_rect_new.top_left.y + line) * surface_size.width + src_rect_new.top_left.x + col);
            uint8_t tab_sc[4];
            tab_sc[3] = *cour_pixel_sc >> 24;
            tab_sc[2] = *cour_pixel_sc >> 16;
            tab_sc[1] = *cour_pixel_sc >> 8;
            tab_sc[0] = *cour_pixel_sc;
            tab_sc[*ir_change] = color->red;
            tab_sc[*ig_change] = color->green;
            tab_sc[*ib_change] = color->blue;
            tab_sc[*ia_change] = color->alpha;

            *cour_pixel_sc = tab_sc[0] | (tab_sc[1] << 8) | (tab_sc[2] << 16) | (tab_sc[3] << 24);
        }
    }
    free(ir_change);
    free(ig_change);
    free(ia_change);
    free(ib_change);

}



/**
* \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
* 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
* 		The surface parameter provides the channel order.
*
* @param	surface		The surface where to store this pixel, provides the channels order.
* @param	color		The color to convert.
*
* @return 			The 32 bit integer corresponding to the color. The alpha component
*				of the color is ignored in the case of surfaces that don't have an
*				alpha channel.
*/
uint32_t		ei_map_rgba		(ei_surface_t surface, ei_color_t color){
    int *ir = malloc(sizeof(int));
    int *ig = malloc(sizeof(int));
    int *ib = malloc(sizeof(int));
    int *ia = malloc(sizeof(int));
    hw_surface_get_channel_indices(surface,ir,ig,ib,ia);
    uint32_t resultat;
    uint8_t tab[4];
    tab[*ir] = color.red;
    tab[*ig] = color.green;
    tab[*ib] = color.blue;
    tab[*ia] = color.alpha;
    resultat = tab[0] | (tab[1] << 8) | (tab[2] << 16) | (tab[3] << 24);
    free(ir);
    free(ig);
    free(ib);
    free(ia);
    return resultat;
}
