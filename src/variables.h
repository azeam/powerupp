typedef struct {
    GtkWidget *g_edit_gfxvolt;
    GtkWidget *g_edit_gfxclock;
    GtkWidget *g_edit_gpupower;
    GtkWidget *g_edit_memmvddvolt;
    GtkWidget *g_edit_memvddcivolt;
    GtkWidget *g_edit_memclock;
    GtkWidget *g_edit_socvolt;
    GtkWidget *g_edit_socclock;

    GtkWidget *g_toggle_gfxvolt;
    GtkWidget *g_toggle_gfxclock;
    GtkWidget *g_toggle_gpupower;
    GtkWidget *g_toggle_memmvddvolt;
    GtkWidget *g_toggle_memvddcivolt;
    GtkWidget *g_toggle_memclock;
    GtkWidget *g_toggle_socvolt;
    GtkWidget *g_toggle_socclock;

    GtkWidget *g_revealer;
    GtkWidget *g_textview_revealer;

    GtkWidget *g_guide;
} app_widgets;

    GtkAdjustment *g_adj_gfxvolt;
    GtkAdjustment *g_adj_gfxclock;
    GtkAdjustment *g_adj_gpupower;
    GtkAdjustment *g_adj_memmvddvolt;
    GtkAdjustment *g_adj_memvddcivolt;
    GtkAdjustment *g_adj_memclock; 
    GtkAdjustment *g_adj_socvolt;
    GtkAdjustment *g_adj_socclock;

    GtkButton *g_btn_active;
    GtkButton *g_btn_defaults;
    GtkButton *g_btn_apply;
    GtkButton *g_btn_perm;
    GtkButton *g_btn_info;

    GtkTextBuffer *g_text_revealer;

    GtkComboBoxText *g_combobox;

    char gfxvolt[512];
    char gfxclock[512];
    char gpupower[512];
    char memmvddvolt[512];
    char memvddcivolt[512];
    char memclock[512];
    char socvolt[512];
    char socclock[512];

    char gfxvoltlimits[512];
    char gfxclocklimits[512];
    char gpupowerlimits[512];
    char memclocklimits[512];

    char uppwrite[512];
    const char* gfxvoltset;
    const char* gfxclockset;
    const char* gpupowerset;
    const char* memmvddvoltset;
    const char* memvddcivoltset;
    const char* memclockset;
    const char* socvoltset;
    const char* socclockset;

    // must always be increased when adding settings, start with 1
    int numberofvalues = 8;
    int numberoflimits = 16;

    // set global order values just to keep track of them easier, start with 1
    int gfxvoltorder = 1;
    int gfxclockorder = 2;
    int gpupowerorder = 3;
    int memmvddvoltorder = 4;
    int memvddcivoltorder = 5;
    int memclockorder = 6;
    int socvoltorder = 7;
    int socclockorder = 8;
    
    int gfxvoltlimitlowerorder = 1;
    int gfxvoltlimitupperorder = 2;
    int gfxclocklimitlowerorder = 3;
    int gfxclocklimitupperorder = 4;
    int gpupowerlimitlowerorder = 5;
    int gpupowerlimitupperorder = 6;
    int memmvddvoltlimitlowerorder = 7;
    int memmvddvoltlimitupperorder = 8;
    int memvddcivoltlimitlowerorder = 9;
    int memvddcivoltlimitupperorder = 10;
    int memclocklimitlowerorder = 11;
    int memclocklimitupperorder = 12;
    int socvoltlimitlowerorder = 13;
    int socvoltlimitupperorder = 14;
    int socclocklimitlowerorder = 15;
    int socclocklimitupperorder = 16;

    // global access to default limits to access from memory instead of file (less code and faster) or adjustment (gives error when toggled to unlimited)
    int gfxvoltlimitlower;
    int gfxvoltlimitupper;
    int gfxclocklimitlower;
    int gfxclocklimitupper;
    int gpupowerlimitlower;
    int gpupowerlimitupper;
    int memmvddvoltlimitlower;
    int memmvddvoltlimitupper;
    int memvddcivoltlimitlower;
    int memvddcivoltlimitupper;
    int memclocklimitlower;
    int memclocklimitupper;
    int socvoltlimitlower;
    int socvoltlimitupper;
    int socclocklimitlower;
    int socclocklimitupper;

    // paths for saving default values under users config folder
    char valuespath[512];
    char limitspath[512];
    struct passwd *p;
    char dirname[256];
    gint card_num;
    