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
  GtkWidget *g_opt_defaults_load;
  GtkWidget *g_opt_persistent_save;
  GtkWidget *g_opt_persistent_del;

} app_widgets;

extern GtkAdjustment *g_adj_gfxvoltmin;
extern GtkAdjustment *g_adj_gfxvolt;
extern GtkAdjustment *g_adj_gfxclock;
extern GtkAdjustment *g_adj_gpupower;
extern GtkAdjustment *g_adj_memmvddvolt0;
extern GtkAdjustment *g_adj_memvddcivolt0;
extern GtkAdjustment *g_adj_memclock0; 
extern GtkAdjustment *g_adj_memmvddvolt1;
extern GtkAdjustment *g_adj_memvddcivolt1;
extern GtkAdjustment *g_adj_memclock1; 
extern GtkAdjustment *g_adj_memmvddvolt2;
extern GtkAdjustment *g_adj_memvddcivolt2;
extern GtkAdjustment *g_adj_memclock2; 
extern GtkAdjustment *g_adj_memmvddvolt;
extern GtkAdjustment *g_adj_memvddcivolt;
extern GtkAdjustment *g_adj_memclock;
extern GtkAdjustment *g_adj_socvoltmin;
extern GtkAdjustment *g_adj_socvolt;
extern GtkAdjustment *g_adj_socclock;
extern GtkAdjustment *g_adj_voltoffset;

extern GtkButton *g_btn_active;
extern GtkButton *g_btn_defaults;
extern GtkButton *g_btn_apply;
extern GtkButton *g_btn_perm;
extern GtkButton *g_btn_info;

extern GtkLabel *g_lblcurgfxvolt;
extern GtkLabel *g_lblcurgfxclock;
extern GtkLabel *g_lblcurgpupower;
extern GtkLabel *g_lblcursocclock;
extern GtkLabel *g_lblcuredgetemp;
extern GtkLabel *g_lblcurjunctemp;
extern GtkLabel *g_lblcurmemtemp;
extern GtkLabel *g_lblcurfanspeed;
extern GtkLabel *g_lblcurmemclock;

extern GtkLevelBar *g_lvlgpuload;
extern GtkLevelBar *g_lvlmemload;

extern GtkTextBuffer *g_text_revealer;

extern GtkComboBoxText *g_combobox;
  
extern char uppwrite[512];
extern char uppdump[600];

extern const char* gfxvoltminset;
extern const char* gfxvoltset;
extern const char* gfxclockset;
extern const char* gpupowerset;
extern const char* memmvddvoltset0;
extern const char* memvddcivoltset0;
extern const char* memclockset0;
extern const char* memmvddvoltset1;
extern const char* memvddcivoltset1;
extern const char* memclockset1;
extern const char* memmvddvoltset2;
extern const char* memvddcivoltset2;
extern const char* memclockset2;
extern const char* memmvddvoltset;
extern const char* memvddcivoltset;
extern const char* memclockset;
extern const char* socvoltminset;
extern const char* socvoltset;
extern const char* socclockset;
extern const char* voltoffsetset;

extern int numberofvalues;
extern int numberoflimits;

  // global access to default limits to access from memory instead of file (less code and faster) or adjustment (gives error when toggled to unlimited)
extern int gfxvoltlimitlower;
extern int gfxvoltlimitupper;
extern int gfxvoltminlimitlower;
extern int gfxvoltminlimitupper;
extern int gfxclocklimitlower;
extern int gfxclocklimitupper;
extern int gpupowerlimitlower;
extern int gpupowerlimitupper;
extern int memmvddvoltlimitlower;
extern int memmvddvoltlimitupper;
extern int memvddcivoltlimitlower;
extern int memvddcivoltlimitupper;
extern int memclocklimitlower;
extern int memclocklimitupper;
extern int socvoltlimitlower;
extern int socvoltlimitupper;
extern int socvoltminlimitlower;
extern int socvoltminlimitupper;
extern int socclocklimitlower;
extern int socclocklimitupper;
extern int voltoffsetlimitlower;
extern int voltoffsetlimitupper;
extern int memmvddvoltlimitlower0;
extern int memmvddvoltlimitupper0;
extern int memvddcivoltlimitlower0;
extern int memvddcivoltlimitupper0;
extern int memclocklimitlower0;
extern int memclocklimitupper0;
extern int memmvddvoltlimitlower1;
extern int memmvddvoltlimitupper1;
extern int memvddcivoltlimitlower1;
extern int memvddcivoltlimitupper1;
extern int memclocklimitlower1;
extern int memclocklimitupper1;
extern int memmvddvoltlimitlower2;
extern int memmvddvoltlimitupper2;
extern int memvddcivoltlimitlower2;
extern int memvddcivoltlimitupper2;
extern int memclocklimitlower2;
extern int memclocklimitupper2;

  // paths
extern char defsettingspath[600];
extern char username[256];
extern char configpath[512];
extern char hwmonpath[512];
extern const char* tempdirectory;
extern char upppath[256];
extern int gl_revtable;
extern int card_num;
extern char getvalues_navi10[2048];
extern char getlimits_navi10[2048];
 
#endif