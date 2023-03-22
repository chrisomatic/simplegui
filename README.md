# Simple GUI

This is a simple GUI library written in C

# Example Code

```C

    #include "imgui.h"


    void draw()
    {
        imgui_begin_panel("Demo", 10,10);

            imgui_text_sized(28, "Demo");
            imgui_text("Hello, My name is %s", "Chris");

            if(imgui_button("Test Button"))
            {
                num_clicks++;
            }

            imgui_text_colored(0xFFFFFFFF, "Num clicks: %d", num_clicks);

        imgui_end();

    }
```

# How to run

```bash

# make sure you have gcc, glfw
./run.sh

```
