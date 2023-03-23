#include "headers.h"
#include "main.h"
#include "window.h"
#include "shader.h"
#include "timer.h"
#include "gfx.h"
#include "math2d.h"
#include "imgui.h"
#include "log.h"

// Settings
// #define VIEW_WIDTH   1812
// #define VIEW_HEIGHT  1359

// =========================
// Global Vars
// =========================

bool paused = false;
Timer main_timer = {0};

// =========================
// Function Prototypes
// =========================

void start_gui();
void init();
void deinit();
//void simulate(double);
void draw();

// =========================
// Main Loop
// =========================

int main(int argc, char* argv[])
{
    start_gui();
    return 0;
}

void start_gui()
{
    LOGI("--------------");
    LOGI("Starting GUI");
    LOGI("--------------");

    time_t t;
    srand((unsigned) time(&t));

    init();

    timer_set_fps(&main_timer,TARGET_FPS);
    timer_begin(&main_timer);

    double curr_time = timer_get_time();
    double new_time  = 0.0;
    double accum = 0.0;

    const double dt = 1.0/TARGET_FPS;

    // main game loop
    for(;;)
    {
        new_time = timer_get_time();
        double frame_time = new_time - curr_time;
        curr_time = new_time;

        accum += frame_time;

        window_poll_events();
        if(window_should_close())
        {
            break;
        }

        while(accum >= dt)
        {
            //simulate(dt);
            accum -= dt;
        }

        draw();

        timer_wait_for_frame(&main_timer);
        window_swap_buffers();
        window_mouse_update_actions();
    }

    deinit();
}

void init()
{
    init_timer();
    log_init(0);

    LOGI("resolution: %d %d",VIEW_WIDTH, VIEW_HEIGHT);
    bool success = window_init(VIEW_WIDTH, VIEW_HEIGHT);

    if(!success)
    {
        fprintf(stderr,"Failed to initialize window!\n");
        exit(1);
    }

    LOGI("Initializing...");

    LOGI(" - Shaders.");
    shader_load_all();

    LOGI(" - Graphics.");
    gfx_init(VIEW_WIDTH, VIEW_HEIGHT);
}

void deinit()
{
    shader_deinit();
    window_deinit();
}

// variables used to store state
// demo vars
int num_clicks = 0;
uint32_t color1 = 0x00FE2225;
uint32_t color2 = 0x002468F2;
float v1 = 0.0;
float v2 = 0.0;
char name[20] = {'H','e','l','l','o','\0'};
char something[20] = {0};
bool my_check = true;
int ri = 10;
bool toggle = false;
bool thing1 = false, thing2 = false;

void draw()
{
    gfx_clear_buffer(50,50,50);

    imgui_draw_demo(10,10);

    imgui_begin_panel("Theme",500,10);
        imgui_theme_editor();
    imgui_end();

#if 0
    imgui_begin_panel("Editor", 10, 10);

        imgui_store_theme();
        imgui_text_sized(28,"Test");
        if(imgui_button("Hello"))
        {
            num_clicks++;
        }
        imgui_text_colored(0x00FFFF00,"Num clicks: %d",num_clicks);

        imgui_restore_theme();
    Vector2f size = imgui_end();
#endif
}
