#include "headers.h"
#include "main.h"
#include "window.h"
#include "shader.h"
#include "timer.h"
#include "gfx.h"
#include "math2d.h"
#include "imgui.h"
#include "log.h"

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
    return 0;
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

    imgui_load_theme("nord_deep.theme");
}

void deinit()
{
    shader_deinit();
    window_deinit();
}

int command_runner(char* cmd, char output[100][100+1])
{
    //int status = system(cmd);
    //int exitcode = status / 256;

    LOGI("Running command: %s\n",cmd);

    FILE * fp = popen(cmd, "r");

    if(!fp)
    {
        LOGE("Command failed to run");
        return 0;
    }

    char line[100+1] = {0};
    int line_count = 0;

    for(;;)
    {
        if(fgets(line,100,fp) == NULL)
            break;

        memset(output[line_count],0,100);
        memcpy(output[line_count++],line,MIN(100,strlen(line)-1));
    }

    fclose(fp);

    return line_count;
}

static char lines[100][100+1] = {0};
static int line_count = 0;
static uint32_t background_color = 0x00303030;

int num_clicks = 0;
float v1 = 0.0;

void draw()
{
    uint8_t r = background_color >> 16;
    uint8_t g = background_color >> 8;
    uint8_t b = background_color >> 0;
    gfx_clear_buffer(r,g,b);

    imgui_draw_demo(10,10);

    /*
    static char command[100] = {0};
    imgui_begin_panel("Command Runner", 500, 10);
        imgui_theme_selector();
        imgui_text_sized(24,"Command Runner");
        imgui_horizontal_line();
        imgui_horizontal_begin();

            imgui_text_box("Command",command,IM_ARRAYSIZE(command));
            if(imgui_button("Run"))
            {
                line_count = command_runner(command, lines);
            }

        imgui_horizontal_end();

        imgui_text_sized(16, "Output");
        for(int i = 0; i < line_count; ++i)
        {
            imgui_text("%03d: %s",i, lines[i]);
        }

    imgui_end();
    */

    imgui_begin_panel("Theme",500,10, false);
        imgui_color_picker("Window Background Color", &background_color);
        imgui_theme_editor();
    imgui_end();
}
