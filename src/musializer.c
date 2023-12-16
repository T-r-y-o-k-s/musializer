#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <complex.h>
#include <math.h>

#include <raylib.h>

#ifndef _WIN32
#include <signal.h> // needed for sigaction()
#endif // _WIN32

//@argv
char *shift_args(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    (*argv) += 1;
    (*argc) -= 1;
    return result;
}

#include "./hotreload.h"

//@argv
int main(int argc, char** argv)
{
#ifndef _WIN32
    // NOTE: This is needed because if the pipe between Musializer and FFmpeg breaks
    // Musializer will receive SIGPIPE on trying to write into it. While such behavior
    // makes sense for command line utilities, Musializer is a relatively friendly GUI
    // application that is trying to recover from such situations.
    struct sigaction act = {0};
    act.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &act, NULL);
#endif // _WIN32

    if (!reload_libplug()) return 1;

    //@argv
    shift_args(&argc, &argv);

    Image logo = LoadImage("./resources/logo/logo-256.png");
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    size_t factor = 80;
    InitWindow(factor*16, factor*9, "Musializer");
    SetWindowIcon(logo);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    InitAudioDevice();

    plug_init();

    //@argv
    if (argc > 0) {
         plug_load_music(argv, argc);
    }
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_H)) {
            void *state = plug_pre_reload();
            if (!reload_libplug()) return 1;
            plug_post_reload(state);
        }
        plug_update();
    }

    CloseAudioDevice();
    CloseWindow();
    UnloadImage(logo);

    return 0;
}
