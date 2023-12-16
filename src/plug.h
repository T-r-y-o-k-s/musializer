#ifndef PLUG_H_
#define PLUG_H_

#define LIST_OF_PLUGS \
    PLUG(plug_init, void, void) \
    PLUG(plug_pre_reload, void*, void) \
    PLUG(plug_post_reload, void, void*) \
    PLUG(plug_update, void, void) \
    PLUG(plug_load_music, void, char**, size_t)
    // I have no idea why, but it won't work with void: "error: 'void' must be the first and only parameter if specified"
    //@argv

#define PLUG(name, ret, ...) typedef ret (name##_t)(__VA_ARGS__);
LIST_OF_PLUGS
#undef PLUG

#endif // PLUG_H_
