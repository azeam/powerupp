#ifndef VARIABLES_H_
#define VARIABLES_H_

typedef struct {
    GtkWidget *g_edit_gfxvoltmin;
    GtkWidget *g_edit_gfxvolt;
    GtkWidget *g_edit_gfxclock;
    GtkWidget *g_edit_gpupower;
    GtkWidget *g_edit_memmvddvolt0;
    GtkWidget *g_edit_memvddcivolt0;
    GtkWidget *g_edit_memclock0;
    GtkWidget *g_edit_memmvddvolt1;
    GtkWidget *g_edit_memvddcivolt1;
    GtkWidget *g_edit_memclock1;
    GtkWidget *g_edit_memmvddvolt2;
    GtkWidget *g_edit_memvddcivolt2;
    GtkWidget *g_edit_memclock2;
    GtkWidget *g_edit_memmvddvolt;
    GtkWidget *g_edit_memvddcivolt;
    GtkWidget *g_edit_memclock;
    GtkWidget *g_edit_socvoltmin;
    GtkWidget *g_edit_socvolt;
    GtkWidget *g_edit_socclock;
    GtkWidget *g_edit_voltoffset;

    GtkWidget *g_toggle_limits;
    
    GtkWidget *g_revealer;
    GtkWidget *g_textview_revealer;

    GtkWidget *g_guide;

    GtkWidget *g_opt_profile_load;
    GtkWidget *g_opt_profile_save;

} app_widgets;

    GtkAdjustment *g_adj_gfxvoltmin;
    GtkAdjustment *g_adj_gfxvolt;
    GtkAdjustment *g_adj_gfxclock;
    GtkAdjustment *g_adj_gpupower;
    GtkAdjustment *g_adj_memmvddvolt0;
    GtkAdjustment *g_adj_memvddcivolt0;
    GtkAdjustment *g_adj_memclock0; 
    GtkAdjustment *g_adj_memmvddvolt1;
    GtkAdjustment *g_adj_memvddcivolt1;
    GtkAdjustment *g_adj_memclock1; 
    GtkAdjustment *g_adj_memmvddvolt2;
    GtkAdjustment *g_adj_memvddcivolt2;
    GtkAdjustment *g_adj_memclock2; 
    GtkAdjustment *g_adj_memmvddvolt;
    GtkAdjustment *g_adj_memvddcivolt;
    GtkAdjustment *g_adj_memclock;
    GtkAdjustment *g_adj_socvoltmin;
    GtkAdjustment *g_adj_socvolt;
    GtkAdjustment *g_adj_socclock;
    GtkAdjustment *g_adj_voltoffset;

    GtkButton *g_btn_active;
    GtkButton *g_btn_defaults;
    GtkButton *g_btn_apply;
    GtkButton *g_btn_perm;
    GtkButton *g_btn_info;

    GtkLabel *g_lblcurgfxvolt;
    GtkLabel *g_lblcurgfxclock;
    GtkLabel *g_lblcurgpupower;
    GtkLabel *g_lblcursocclock;
    GtkLabel *g_lblcuredgetemp;
    GtkLabel *g_lblcurjunctemp;
    GtkLabel *g_lblcurmemtemp;
    GtkLabel *g_lblcurfanspeed;
    GtkLabel *g_lblcurmemclock;

    GtkLevelBar *g_lvlgpuload;
    GtkLevelBar *g_lvlmemload;

    GtkTextBuffer *g_text_revealer;

    GtkComboBoxText *g_combobox;

    char pp_gfxvoltmin[512];
    char pp_gfxvolt[512];
    char pp_gfxclock[512];
    char pp_gpupower[512];
    char pp_memmvddvolt0[512];
    char pp_memvddcivolt0[512];
    char pp_memclock0[512];
    char pp_memmvddvolt1[512];
    char pp_memvddcivolt1[512];
    char pp_memclock1[512];
    char pp_memmvddvolt2[512];
    char pp_memvddcivolt2[512];
    char pp_memclock2[512];
    char pp_memmvddvolt[512];
    char pp_memvddcivolt[512];
    char pp_memclock[512];
    char pp_socvoltmin[512];
    char pp_socvolt[512];
    char pp_socclock[512];
    char pp_voltoffset[512];

    char pp_gfxvoltlimitlower[512];
    char pp_gfxvoltlimitupper[512];
    char pp_gfxclocklimitlower[512];
    char pp_gfxclocklimitupper[512];
    char pp_gpupowerlimitlower[512];
    char pp_gpupowerlimitupper[512];
    char pp_memclocklimitlower[512];
    char pp_memclocklimitupper[512];
    
    char uppwrite[512];
    char uppdump[600];

    const char* gfxvoltminset;
    const char* gfxvoltset;
    const char* gfxclockset;
    const char* gpupowerset;
    const char* memmvddvoltset0;
    const char* memvddcivoltset0;
    const char* memclockset0;
    const char* memmvddvoltset1;
    const char* memvddcivoltset1;
    const char* memclockset1;
    const char* memmvddvoltset2;
    const char* memvddcivoltset2;
    const char* memclockset2;
    const char* memmvddvoltset;
    const char* memvddcivoltset;
    const char* memclockset;
    const char* socvoltminset;
    const char* socvoltset;
    const char* socclockset;
    const char* voltoffsetset;

    int numberofvalues;
    int numberoflimits;

    // global access to default limits to access from memory instead of file (less code and faster) or adjustment (gives error when toggled to unlimited)
    int gfxvoltlimitlower;
    int gfxvoltlimitupper;
    int gfxvoltminlimitlower;
    int gfxvoltminlimitupper;
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
    int socvoltminlimitlower;
    int socvoltminlimitupper;
    int socclocklimitlower;
    int socclocklimitupper;
    int voltoffsetlimitlower;
    int voltoffsetlimitupper;
    int memmvddvoltlimitlower0;
    int memmvddvoltlimitupper0;
    int memvddcivoltlimitlower0;
    int memvddcivoltlimitupper0;
    int memclocklimitlower0;
    int memclocklimitupper0;
    int memmvddvoltlimitlower1;
    int memmvddvoltlimitupper1;
    int memvddcivoltlimitlower1;
    int memvddcivoltlimitupper1;
    int memclocklimitlower1;
    int memclocklimitupper1;
    int memmvddvoltlimitlower2;
    int memmvddvoltlimitupper2;
    int memvddcivoltlimitlower2;
    int memvddcivoltlimitupper2;
    int memclocklimitlower2;
    int memclocklimitupper2;

    // paths
    char defsettingspath[600];
    char username[256];
    char configpath[512];
    gint card_num;
    char hwmonpath[512];
    const char* tempdirectory;
    char ftempname[256];
    char pythonpath[256];
    int gl_revtable;

#endif