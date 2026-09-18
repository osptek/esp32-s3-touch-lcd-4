#include "lvgl.h"
#include <stdlib.h>

LV_IMG_DECLARE(bird_img1)
LV_IMG_DECLARE(bird_img2)
LV_IMG_DECLARE(bird_img3)
LV_IMG_DECLARE(bird_bg_img)
LV_IMG_DECLARE(pipe_img)
LV_IMG_DECLARE(bird_start_img)
LV_IMG_DECLARE(bird_grass_img)

typedef struct
{
    lv_obj_t * pipe_obj;
    int32_t  x1;
    int32_t  y1;
    int32_t  x2;
    int32_t  y2;
	bool  alive;
}pipe_type_def;


static pipe_type_def pipe_lib[8];

static lv_obj_t * screen,* screen1,* bird_obj,* bg,* start_btn,* exit_btn,* grass_bg,*score_lable;
static lv_anim_t bird_anim;
static lv_timer_t * t1,* t2;
float screen_w_ratio,screen_h_ratio,screen_ratio;
int32_t  screen_w,screen_h,screen_size,score=0;
static const lv_img_dsc_t * bird_img_lib[]={&bird_img1,&bird_img2,&bird_img3,&bird_img2,};

static void x_move_cb(void * var, int32_t v);
static void y_move_cb(void * var, int32_t v);
static void scr_click_cb(lv_event_t * e);
static void move_done_cb(lv_anim_t * a);
static int32_t lv_anim_path_down(const lv_anim_t * a);
static void set_angle_cb(void * var, int32_t v);
static void timer_cb1(lv_timer_t * t);
static void timer_cb2(lv_timer_t * t);
static void game_start(lv_event_t * e);
static void exit_game();
static void restart_game();
static void bird_dead(int32_t y);


void bird()
{
    lv_memset_00(pipe_lib,sizeof(pipe_lib));

    screen_w=lv_disp_get_hor_res(lv_disp_get_default());
    screen_h=lv_disp_get_ver_res(lv_disp_get_default());

    if(screen_w>=screen_h)
    {
        screen_size=screen_h;
    }
    else{
            screen_size=screen_w;
    }

    screen_ratio=(float)screen_size/600;

    screen1=lv_tileview_create(lv_scr_act());
    lv_obj_clear_flag(screen1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(screen1,lv_color_hex(0), LV_PART_MAIN);

    screen=lv_tileview_create(screen1);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(screen,lv_color_hex(0x70c5cf), LV_PART_MAIN);
    lv_obj_add_event_cb(screen,scr_click_cb,LV_EVENT_PRESSED,0);
    lv_obj_set_size(screen,screen_size,screen_size);
    lv_obj_center(screen);

    bg=lv_img_create(screen);
    lv_img_set_src(bg, &bird_bg_img);
    lv_obj_align(bg,LV_ALIGN_BOTTOM_LEFT,0,-30);
    lv_obj_set_size(bg,screen_size,150);

    grass_bg=lv_img_create(screen);
    lv_img_set_src(grass_bg, &bird_grass_img);
    lv_obj_set_align(grass_bg,LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_size(grass_bg,screen_size,30);

    bird_obj=lv_img_create(screen);
    lv_img_set_src(bird_obj, &bird_img1);
    lv_obj_set_pos(bird_obj,screen_size/4,screen_size/2);
    lv_img_set_zoom(bird_obj,256*screen_ratio);
    lv_img_set_angle(bird_obj,-100);

    start_btn=lv_img_create(screen);
    lv_img_set_src(start_btn, &bird_start_img);
    lv_obj_center(start_btn);
    lv_img_set_zoom(start_btn,256*screen_ratio);
    lv_obj_add_flag(start_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(start_btn,game_start,LV_EVENT_RELEASED,0);

    exit_btn=lv_label_create(screen);
    lv_obj_set_align(exit_btn,LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(exit_btn,"< EXIT  ");
    lv_obj_add_flag(exit_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(exit_btn,exit_game,LV_EVENT_RELEASED,0);

    score_lable=lv_label_create(screen);
    lv_obj_align(score_lable,LV_ALIGN_TOP_MID,0,100*screen_ratio);
    lv_label_set_text_fmt(score_lable,"%d",(int)score);
    lv_obj_set_style_text_color(score_lable,lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(score_lable,&lv_font_montserrat_32, LV_PART_MAIN);

    t1=lv_timer_create(timer_cb1, 80, 0);
    t2=lv_timer_create(timer_cb2, 20/screen_ratio, 0);
    lv_timer_pause(t2);

}


static void game_start(lv_event_t * e)
{
    lv_memset_00(pipe_lib,sizeof(pipe_lib));
    lv_obj_add_flag(start_btn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(screen, LV_OBJ_FLAG_CLICKABLE);

    score=0;

    lv_label_set_text_fmt(score_lable,"%d",0);

    lv_timer_resume(t2);

}



static void scr_click_cb(lv_event_t * e)
{

    lv_anim_del(bird_obj,y_move_cb);
    lv_anim_del(bird_obj,set_angle_cb);

    lv_obj_refr_pos(bird_obj);
    int32_t  y=lv_obj_get_y(bird_obj);

    lv_img_set_angle(bird_obj,-250);

    lv_anim_init(&bird_anim);
    lv_anim_set_var(&bird_anim,bird_obj);
    lv_anim_set_exec_cb(&bird_anim,y_move_cb);
    lv_anim_set_time(&bird_anim,150);
    lv_anim_set_values(&bird_anim,y,y-(float)80*screen_ratio);
    lv_anim_set_ready_cb(&bird_anim,move_done_cb);
    lv_anim_start(&bird_anim);

}


static void timer_cb1(lv_timer_t * t)
{
    static int32_t i=0;

    lv_img_set_src(bird_obj, bird_img_lib[i]);

    i++;
    if(i>3){i=0;}

}


static void timer_cb2(lv_timer_t * t)
{
    static int32_t i=0,bx=0,gx=0;
    int j;

    bx--;
    gx-=4;

    lv_img_set_offset_x(bg,bx);
    lv_img_set_offset_x(grass_bg,gx);

    if(i == 0)
    {
        int32_t d=(rand()%250+50)*screen_ratio;


        for(j=0;j<8;j++)
        {
            if(pipe_lib[j].alive == false)
            {

                pipe_lib[j].pipe_obj=lv_img_create(screen);
                pipe_lib[j].alive=true;
                pipe_lib[j].x1=screen_size;
                pipe_lib[j].y1=d-400*screen_ratio;
                pipe_lib[j].x2=pipe_lib[j].x1+80*screen_ratio;
                pipe_lib[j].y2=pipe_lib[j].y1+400*screen_ratio;
                lv_img_set_src(pipe_lib[j].pipe_obj, &pipe_img);
                lv_obj_set_pos(pipe_lib[j].pipe_obj,pipe_lib[j].x1,pipe_lib[j].y1);
                lv_img_set_pivot(pipe_lib[j].pipe_obj,0,0);
                lv_img_set_zoom(pipe_lib[j].pipe_obj,256*screen_ratio);

                lv_obj_move_foreground(exit_btn);
                lv_obj_move_foreground(score_lable);

                score++;
                if(score>2)
                {
                     lv_label_set_text_fmt(score_lable,"%d",(int)(score-2));
                }
                break;
            }

        }

        for(j=0;j<8;j++)
        {
            if(pipe_lib[j].alive == false)
            {

                pipe_lib[j].pipe_obj=lv_img_create(screen);
                pipe_lib[j].alive=true;
                pipe_lib[j].x1=screen_size;
                pipe_lib[j].y1=d+200*screen_ratio;
                pipe_lib[j].x2=pipe_lib[j].x1+80*screen_ratio;
                pipe_lib[j].y2=pipe_lib[j].y1+400*screen_ratio;
                lv_img_set_src(pipe_lib[j].pipe_obj, &pipe_img);
                lv_obj_set_pos(pipe_lib[j].pipe_obj,pipe_lib[j].x1,pipe_lib[j].y1);
                lv_img_set_pivot(pipe_lib[j].pipe_obj,0,0);
                lv_img_set_zoom(pipe_lib[j].pipe_obj,256*screen_ratio);

                lv_obj_move_foreground(grass_bg);
                break;
            }

        }

    }

        for(j=0;j<8;j++)
        {
            if(pipe_lib[j].alive == true)
            {

                pipe_lib[j].x1-=4;
                pipe_lib[j].x2-=4;

                if(pipe_lib[j].x1<-80*screen_ratio)
                {
                    lv_obj_del(pipe_lib[j].pipe_obj);
                    pipe_lib[j].alive = false;
                }
                else{
                lv_obj_set_x(pipe_lib[j].pipe_obj,pipe_lib[j].x1);
                }
            }
        }

        int32_t x=lv_obj_get_x(bird_obj);
        int32_t y=lv_obj_get_y(bird_obj);

        if(y<0 || y> screen_size-41*screen_ratio-30)
        {
            bird_dead(y);
            return;
        }

        for(j=0;j<8;j++)
        {
            if(pipe_lib[j].alive == true)
            {
                if((x+57*screen_ratio)>pipe_lib[j].x1 && x<pipe_lib[j].x2 && (y+41*screen_ratio)>pipe_lib[j].y1 && y< pipe_lib[j].y2)
                {
                    bird_dead(y);
                    return;
                }
            }
        }

    i++;
    if(i>60){i=0;}
}


static void bird_dead(int32_t y)
{
    int32_t a=lv_img_get_angle(bird_obj);

    lv_timer_pause(t1);
    lv_timer_pause(t2);
    lv_anim_del(bird_obj,y_move_cb);
    lv_anim_del(bird_obj,set_angle_cb);
    lv_obj_move_foreground(bird_obj);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_CLICKABLE);

    lv_anim_init(&bird_anim);
    lv_anim_set_var(&bird_anim,bird_obj);
    lv_anim_set_exec_cb(&bird_anim,y_move_cb);
    lv_anim_set_time(&bird_anim,800*(screen_size-y)/screen_size);
    lv_anim_set_path_cb(&bird_anim,lv_anim_path_ease_in);
    lv_anim_set_values(&bird_anim,y,screen_size-41*screen_ratio-30);
    lv_anim_start(&bird_anim);

    lv_anim_init(&bird_anim);
    lv_anim_set_var(&bird_anim,bird_obj);
    lv_anim_set_exec_cb(&bird_anim,set_angle_cb);
    lv_anim_set_time(&bird_anim,800*(screen_size-y)/screen_size);
    lv_anim_set_values(&bird_anim,0,90);
    lv_anim_set_path_cb(&bird_anim,lv_anim_path_ease_in);
    lv_anim_start(&bird_anim);


    lv_anim_init(&bird_anim);
    lv_anim_set_time(&bird_anim,10);
    lv_anim_set_values(&bird_anim,0,1);
    lv_anim_set_delay(&bird_anim,2000);
    lv_anim_set_ready_cb(&bird_anim,restart_game);
    lv_anim_start(&bird_anim);

}


static void restart_game()
{
    for(int32_t j=0;j<8;j++)
    {
        if(pipe_lib[j].alive == true)
        {
                lv_obj_del(pipe_lib[j].pipe_obj);
                pipe_lib[j].alive = false;
        }
    }

    lv_obj_set_pos(bird_obj,screen_size/4,screen_size/2);
    lv_img_set_angle(bird_obj,-100);
    lv_timer_resume(t1);
    lv_obj_clear_flag(start_btn, LV_OBJ_FLAG_HIDDEN);

}


static void move_done_cb(lv_anim_t * a)
{
    lv_obj_refr_pos(bird_obj);
    int32_t  y=lv_obj_get_y(bird_obj);

    lv_anim_init(&bird_anim);
    lv_anim_set_var(&bird_anim,bird_obj);
    lv_anim_set_exec_cb(&bird_anim,y_move_cb);
    lv_anim_set_time(&bird_anim,1000);
    lv_anim_set_path_cb(&bird_anim,lv_anim_path_down);
    lv_anim_set_values(&bird_anim,y,y+screen_size);
    lv_anim_start(&bird_anim);

    lv_anim_init(&bird_anim);
    lv_anim_set_var(&bird_anim,bird_obj);
    lv_anim_set_exec_cb(&bird_anim,set_angle_cb);
    lv_anim_set_time(&bird_anim,1000);
    lv_anim_set_values(&bird_anim,-25,90);
    lv_anim_set_path_cb(&bird_anim,lv_anim_path_ease_in);
    lv_anim_start(&bird_anim);

}


static void y_move_cb(void * var, int32_t v)
{
	lv_obj_t * xxx=(lv_obj_t *)var;
	lv_obj_set_y(xxx,v);
}

static void set_angle_cb(void * var, int32_t v)
{
	lv_obj_t * xxx=(lv_obj_t *)var;
	lv_img_set_angle(xxx,v*10);
}

static int32_t lv_anim_path_down(const lv_anim_t * a)
{
    uint32_t t = lv_map(a->act_time, 0, a->time, 0, LV_BEZIER_VAL_MAX);
    int32_t step = lv_bezier3(t, 0, 20, 100, LV_BEZIER_VAL_MAX);

    int32_t new_value;
    new_value = step * (a->end_value - a->start_value);
    new_value = new_value >> LV_BEZIER_VAL_SHIFT;
    new_value += a->start_value;

    return new_value;
}


static void exit_game()
{
    lv_anim_del_all();
    lv_timer_del(t1);
    lv_timer_del(t2);
    lv_obj_del(screen1);
}

