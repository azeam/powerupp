#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "variables.h"
#include "active.h"
#include "defaults.h"
#include "apply.h"
#include "save.h"

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
char hwmonpath[512];
const char* tempdirectory;
char upppath[256];
int gl_revtable;
int card_num;
char getvalues_navi[2048];
char getlimits_navi[2048];

int readcurval(char *rcmd) {
  unsigned int val;
  char response[256];
  FILE *fcurvalue = popen(rcmd, "r");
  if(fcurvalue != NULL) {
    if(fgets(response, sizeof response, fcurvalue)){
      val = atoi(response);
    }
  }
  pclose(fcurvalue);
  return val;
}

static gboolean monitoring (gpointer user_data) {
  unsigned int val;
  char valunit[256];
  gdouble memload;
  gdouble gpuload;
  
  if(access(hwmonpath, R_OK) != 0) {
    gtk_label_set_text(GTK_LABEL(g_lblcurgfxclock), "N/A");
    gtk_label_set_text(GTK_LABEL(g_lblcurgfxvolt), "N/A");
    gtk_label_set_text(GTK_LABEL(g_lblcurgpupower), "N/A");
    gtk_label_set_text(GTK_LABEL(g_lblcursocclock), "N/A");
    gtk_label_set_text(GTK_LABEL(g_lblcuredgetemp), "N/A");
    gtk_label_set_text(GTK_LABEL(g_lblcurjunctemp), "N/A");
    gtk_label_set_text(GTK_LABEL(g_lblcurmemtemp), "N/A");
    gtk_label_set_text(GTK_LABEL(g_lblcurfanspeed), "N/A");
    gtk_label_set_text(GTK_LABEL(g_lblcurmemclock), "N/A");
    gtk_level_bar_set_value(GTK_LEVEL_BAR(g_lvlgpuload), 0);
    gtk_level_bar_set_value(GTK_LEVEL_BAR(g_lvlmemload), 0);
    return FALSE;
  }
  char curgfxvolt[600];
  snprintf(curgfxvolt, sizeof(curgfxvolt), "cat %s/in0_input 2>/dev/null | tr -d $'\n'", hwmonpath);
  char curgfxclock[600];
  snprintf(curgfxclock, sizeof(curgfxclock), "cat %s/freq1_input 2>/dev/null | tr -d $'\n'", hwmonpath);
  char curgpupower[600];
  snprintf(curgpupower, sizeof(curgpupower), "cat %s/power1_average 2>/dev/null | tr -d $'\n'", hwmonpath);
  char cursocclock[600];
  snprintf(cursocclock, sizeof(cursocclock), "cat /sys/class/drm/card%d/device/pp_dpm_socclk 2>/dev/null | grep '*' | cut -d' ' -f2 | tr -dc '0-9'", card_num);
  char curedgetemp[600];
  snprintf(curedgetemp, sizeof(curedgetemp), "cat %s/temp1_input 2>/dev/null | tr -d $'\n'", hwmonpath);
  char curjunctemp[600];
  snprintf(curjunctemp, sizeof(curjunctemp), "cat %s/temp2_input 2>/dev/null | tr -d $'\n'", hwmonpath);
  char curmemtemp[600];
  snprintf(curmemtemp, sizeof(curmemtemp), "cat %s/temp3_input 2>/dev/null | tr -d $'\n'", hwmonpath);
  char curfanspeed[600];
  snprintf(curfanspeed, sizeof(curfanspeed), "cat %s/fan1_input 2>/dev/null | tr -d $'\n'", hwmonpath);
  char curmemclock[600];
  snprintf(curmemclock, sizeof(curmemclock), "cat %s/freq2_input 2>/dev/null | tr -d $'\n'", hwmonpath);
  char curgpuload[600];
  snprintf(curgpuload, sizeof(curgpuload), "cat /sys/class/drm/card%d/device/gpu_busy_percent 2>/dev/null | tr -dc '0-9'", card_num);
  char curmemload[600];
  snprintf(curmemload, sizeof(curmemload), "cat /sys/class/drm/card%d/device/mem_busy_percent 2>/dev/null | tr -dc '0-9'", card_num);

  val = readcurval(curgfxclock);
  snprintf(valunit, sizeof(valunit),"%d MHz", val / 1000000);
  gtk_label_set_text(GTK_LABEL(g_lblcurgfxclock), valunit);

  val = readcurval(curgfxvolt);
  snprintf(valunit, sizeof(valunit),"%d mV", val);
  gtk_label_set_text(GTK_LABEL(g_lblcurgfxvolt), valunit);

  val = readcurval(curgpupower);
  snprintf(valunit, sizeof(valunit),"%d W", val / 1000000);
  gtk_label_set_text(GTK_LABEL(g_lblcurgpupower), valunit);

  val = readcurval(cursocclock);
  snprintf(valunit, sizeof(valunit),"%d MHz", val);
  gtk_label_set_text(GTK_LABEL(g_lblcursocclock), valunit);

  val = readcurval(curedgetemp);
  snprintf(valunit, sizeof(valunit),"%d °C", val / 1000);
  gtk_label_set_text(GTK_LABEL(g_lblcuredgetemp), valunit);

  val = readcurval(curjunctemp);
  snprintf(valunit, sizeof(valunit),"%d °C", val / 1000);
  gtk_label_set_text(GTK_LABEL(g_lblcurjunctemp), valunit);

  val = readcurval(curmemtemp);
  snprintf(valunit, sizeof(valunit),"%d °C", val / 1000);
  gtk_label_set_text(GTK_LABEL(g_lblcurmemtemp), valunit);

  val = readcurval(curfanspeed);
  snprintf(valunit, sizeof(valunit),"%d RPM", val);
  gtk_label_set_text(GTK_LABEL(g_lblcurfanspeed), valunit);

  val = readcurval(curmemclock);
  snprintf(valunit, sizeof(valunit),"%d MHz", val / 1000000);
  gtk_label_set_text(GTK_LABEL(g_lblcurmemclock), valunit);
  
  val = readcurval(curgpuload);
  gpuload = val;
  gtk_level_bar_set_value(GTK_LEVEL_BAR(g_lvlgpuload), gpuload);
  
  val = readcurval(curmemload);
  memload = val;
  gtk_level_bar_set_value(GTK_LEVEL_BAR(g_lvlmemload), memload);

  return G_SOURCE_CONTINUE;
}

void prepare_monitoring(char *hwmonprepath) {
  // hwmon subfolder can contain any number in its name, search for subfolders and use the first folder containing hwmon as monitoring path, can't use ls in path when checking for access
  char cwd[512];
  DIR *dp;
  struct dirent *entry;
  struct stat statbuf;
  strcpy(hwmonpath, "");
  
  if((dp = opendir(hwmonprepath)) != NULL) {
    chdir(hwmonprepath);
    while((entry = readdir(dp)) != NULL) {
      lstat(entry->d_name, &statbuf);
      if(S_ISDIR(statbuf.st_mode)) {
        if (strstr(entry->d_name, "hwmon") != NULL) {
          printf("hwmon subdirectory found, chdir %s/\n", entry->d_name);
          chdir(entry->d_name);
          if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Using data from %s for monitoring\n", cwd);
            snprintf(hwmonpath, sizeof(hwmonpath), "%s", cwd);
          }
          break;
        }
      }
    }
  }
  else {
    printf("Can't open directory: %s\n", hwmonprepath); 
  }
  closedir(dp);
  
  // start monitoring
  if(access(hwmonpath, R_OK) == 0) {
    g_timeout_add(200 /* milliseconds */, monitoring, NULL);
  }
  else {
    printf("Can't read hwmon path for card %d, unable to monitor\n", card_num);
  }
}

void setup_gpu_paths_and_options(app_widgets *app_wdgts) {
      char hwmonprepath[256];
      char savedudevpath[512];

      //set "--write" at the end of cmd after testing or remove for testing
      snprintf(uppwrite, sizeof(uppwrite), "--pp-file /sys/class/drm/card%d/device/pp_table set --write", card_num);
      snprintf(savedudevpath, sizeof(savedudevpath), "/etc/udev/rules.d/80-powerupp%d.rules", card_num);
      snprintf(hwmonprepath, sizeof(hwmonprepath), "/sys/class/drm/card%d/device/hwmon", card_num);
      snprintf(defsettingspath, sizeof(defsettingspath), "%s/defaultsettings%d", configpath, card_num);      
      
      gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_profile_load), TRUE);
      gtk_widget_set_sensitive(GTK_WIDGET(g_btn_active), TRUE);
      
      if(access(defsettingspath, F_OK) != -1) {
        gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_defaults_load), TRUE);
      }
      else {
        printf("No default settings found for card %d, not enabling load option\n", card_num);
      }

      if(access(savedudevpath, F_OK) != -1) {
        gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_persistent_del), TRUE);
      }
      else {
        printf("No udev script found for card %d, not enabling remove option\n", card_num);
      }
      prepare_monitoring(hwmonprepath);
} 

// lost patience with glade, connecting manually
static void on_combobox_changed (GtkComboBoxText *combobox, gpointer user_data) {
  app_widgets *app_wdgts = user_data;
  gtk_widget_set_sensitive(GTK_WIDGET(g_btn_active), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_defaults_load), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_persistent_save), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_profile_load), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_profile_save), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_persistent_del), FALSE);

  if (gtk_combo_box_text_get_active_text (g_combobox) != 0) {
    card_num = gtk_combo_box_get_active(GTK_COMBO_BOX(g_combobox));
    gchar *card_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_combobox));
    char navi10[512];
    char bignavi[512];
    snprintf(navi10, sizeof(navi10), "card %d: AMD Radeon 5xxx (Navi 10)", card_num);
    snprintf(bignavi, sizeof(bignavi), "card %d: AMD Radeon 6xxx (Big Navi)", card_num);
    if (strcmp(card_text,navi10) == 0 || strcmp(card_text,bignavi) == 0) {
      // data specific for navi, possible to add data for other GPUs
      // TODO: array needs to be filled with 0 for cards missing certain values
      snprintf(getvalues_navi, sizeof(getvalues_navi), "%s --pp-file /sys/class/drm/card%d/device/pp_table get \
      smc_pptable/MinVoltageGfx \
      smc_pptable/MaxVoltageGfx \
      smc_pptable/FreqTableGfx/1 \
      smc_pptable/SocketPowerLimitAc/0 \
      smc_pptable/MemMvddVoltage/0 \
      smc_pptable/MemMvddVoltage/1 \
      smc_pptable/MemMvddVoltage/2 \
      smc_pptable/MemMvddVoltage/3 \
      smc_pptable/MemVddciVoltage/0 \
      smc_pptable/MemVddciVoltage/1 \
      smc_pptable/MemVddciVoltage/2 \
      smc_pptable/MemVddciVoltage/3 \
      smc_pptable/FreqTableUclk/0 \
      smc_pptable/FreqTableUclk/1 \
      smc_pptable/FreqTableUclk/2 \
      smc_pptable/FreqTableUclk/3 \
      smc_pptable/MinVoltageSoc \
      smc_pptable/MaxVoltageSoc \
      smc_pptable/FreqTableSocclk/1 \
      smc_pptable/qStaticVoltageOffset/0/c \
      ", upppath, card_num);

      //set up write commands
      gfxvoltset = "smc_pptable/MaxVoltageGfx=";
      gfxvoltminset = "smc_pptable/MinVoltageGfx=";
      gpupowerset = "smc_pptable/SocketPowerLimitAc/0=";
      gfxclockset = "smc_pptable/FreqTableGfx/1=";
      memmvddvoltset = "smc_pptable/MemMvddVoltage/3=";
      memvddcivoltset = "smc_pptable/MemVddciVoltage/3=";
      memclockset = "smc_pptable/FreqTableUclk/3=";
      socvoltset = "smc_pptable/MaxVoltageSoc=";
      socvoltminset = "smc_pptable/MinVoltageSoc=";
      socclockset = "smc_pptable/FreqTableSocclk/1=";
      voltoffsetset = "smc_pptable/qStaticVoltageOffset/0/c=";
      memmvddvoltset0 = "smc_pptable/MemMvddVoltage/0=";
      memvddcivoltset0 = "smc_pptable/MemVddciVoltage/0=";
      memclockset0 = "smc_pptable/FreqTableUclk/0=";
      memmvddvoltset1 = "smc_pptable/MemMvddVoltage/1=";
      memvddcivoltset1 = "smc_pptable/MemVddciVoltage/1=";
      memclockset1 = "smc_pptable/FreqTableUclk/1=";
      memmvddvoltset2 = "smc_pptable/MemMvddVoltage/2=";
      memvddcivoltset2 = "smc_pptable/MemVddciVoltage/2=";
      memclockset2 = "smc_pptable/FreqTableUclk/2=";

      //set per card, should make it easier to add cards that don't have all settings
      numberofvalues = 20;
      numberoflimits = 40;

      setup_gpu_paths_and_options(app_wdgts);
    }

    if (strcmp(card_text,navi10) == 0) {
      gl_revtable = 12;
        // 0=gfxclock, 7=gfxvolt, 9=powerlimit, 8=memclock
          snprintf(getlimits_navi, sizeof(getlimits_navi), "%s --pp-file /sys/class/drm/card%d/device/pp_table get \
          overdrive_table/max/0 \
          overdrive_table/max/7 \
          overdrive_table/max/9 \
          overdrive_table/max/8 \
          overdrive_table/min/0 \
          overdrive_table/min/7 \
          overdrive_table/min/9 \
          overdrive_table/min/8 \
          ", upppath, card_num);
    }
    else if (strcmp(card_text,bignavi) == 0) {
      gl_revtable = 15;
      snprintf(getlimits_navi, sizeof(getlimits_navi), "%s --pp-file /sys/class/drm/card%d/device/pp_table get \
      overdrive_table/max/0 \
      overdrive_table/max/7 \
      overdrive_table/max/8 \
      overdrive_table/max/6 \
      overdrive_table/min/0 \
      overdrive_table/min/7 \
      overdrive_table/min/8 \
      overdrive_table/min/6 \
      ", upppath, card_num);
    }
    g_free(card_text);
  }
}

void on_opt_profile_save_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts) {
  gint response;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  GtkWidget *topwidget = gtk_widget_get_toplevel(app_wdgts->g_opt_profile_save);
  GtkWidget *dialog;
  if (GTK_IS_WINDOW(topwidget)) {
    GtkWindow *window = (GTK_WINDOW(topwidget));
    dialog = gtk_file_chooser_dialog_new("Save profile",
                                        window,
                                        action,
                                        ("_Cancel"),
                                        GTK_RESPONSE_CANCEL,
                                        ("_Save"),
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);
    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
    // considered bad practice since it should automatically choose a good folder, it doesn't though...
    gtk_file_chooser_set_current_folder(chooser, configpath);
    gtk_file_chooser_set_current_name (chooser,("settings_profile"));
    response = gtk_dialog_run (GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(chooser);
        const char *key_data = values_to_keyfile(app_wdgts);
        g_autoptr(GError) error = NULL;
        g_autoptr(GKeyFile) key_file = g_key_file_new();
        if (g_key_file_load_from_data (key_file, key_data, -1, G_KEY_FILE_NONE, &error)) {
          if (!g_key_file_save_to_file (key_file, filename, &error)) {
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error saving profile", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
          }
          else {
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Profile successfully saved", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_defaults_load), TRUE);
          }
        }
        else {
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error reading current values", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_defaults_load), TRUE);
        }
        g_key_file_free (key_file);
        g_free(filename);
      }
      gtk_widget_destroy (dialog);
  }
}

void on_opt_profile_load_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts) {
  gint response;
  GtkFileChooser *chooser;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  GtkWidget *topwidget = gtk_widget_get_toplevel(app_wdgts->g_opt_profile_load);
  GtkWidget *dialog;
  if (GTK_IS_WINDOW(topwidget)) {
    GtkWindow *window = (GTK_WINDOW(topwidget));
    dialog = gtk_file_chooser_dialog_new("Load profile",
                                        window,
                                        action,
                                        ("_Cancel"),
                                        GTK_RESPONSE_CANCEL,
                                        ("_Open"),
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);
    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_current_folder(chooser, configpath);
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(chooser);
        char settingspath[600];
        snprintf(settingspath, sizeof(settingspath), "%s", filename);
        int readerror = 0;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), FALSE);
        if (set_limits_from_file(settingspath) == 1) {
          readerror = 1;
        }
        else {
          if (set_values_from_file(settingspath, app_wdgts) == 1) {
            readerror = 1;
          }
        }
        if (readerror == 1) {
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error loading saved values", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), FALSE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_persistent_save), FALSE);
        }
        else {
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Saved profile loaded", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_persistent_save), TRUE); 
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_profile_save), TRUE); 
        }
        g_free(filename);
      }
      gtk_widget_destroy (dialog);
  }
}

void on_btn_revealer_clicked(GtkButton *button, app_widgets *app_wdgts) {
  gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), FALSE);
}

void on_toggle_limits_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
  if (gtk_toggle_button_get_active(togglebutton)){
    // set limits to "max"
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvoltmin), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvoltmin), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxclock), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxclock), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gpupower), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gpupower), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvolt), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvolt), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvoltmin), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvoltmin), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_voltoffset), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_voltoffset), -9999);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock0), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock0), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt0), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt0), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt0), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt0), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock1), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock1), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt1), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt1), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt1), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt1), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock2), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock2), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt2), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt2), 0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt2), 9999);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt2), 0);
  }
  else {
    // set limits to pp table limits (and values where no limits exist)
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), socclocklimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), socclocklimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), socvoltlimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), socvoltlimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvoltmin), socvoltminlimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvoltmin), socvoltminlimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock), memclocklimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock), memclocklimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), memvddcivoltlimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), memvddcivoltlimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), memmvddvoltlimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), memmvddvoltlimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxclock), gfxclocklimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxclock), gfxclocklimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gpupower), gpupowerlimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gpupower), gpupowerlimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvolt), gfxvoltlimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvolt), gfxvoltlimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvoltmin), gfxvoltminlimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvoltmin), gfxvoltminlimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_voltoffset), voltoffsetlimitupper);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_voltoffset), voltoffsetlimitlower);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock0), memclocklimitupper0);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock0), memclocklimitlower0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt0), memvddcivoltlimitupper0);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt0), memvddcivoltlimitlower0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt0), memmvddvoltlimitupper0);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt0), memmvddvoltlimitlower0);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock1), memclocklimitupper1);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock1), memclocklimitlower1);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt1), memvddcivoltlimitupper1);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt1), memvddcivoltlimitlower1);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt1), memmvddvoltlimitupper1);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt1), memmvddvoltlimitlower1);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock2), memclocklimitupper2);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock2), memclocklimitlower2);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt2), memvddcivoltlimitupper2);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt2), memvddcivoltlimitlower2);
    gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt2), memmvddvoltlimitupper2);
    gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt2), memmvddvoltlimitlower2);

    //spin button should be updated if value is set higher than limit or it will stay above limit when toggling
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvoltmin));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvoltmin));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_voltoffset));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt0));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock0));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt0));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt1));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock1));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt1));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt2));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock2));
    gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt2));
  }
}

void get_home_path(app_widgets *widgets) {
  char localpath[512];
  char localupppath[600];
  struct passwd *p;
  p = getpwuid(getuid());
  if (p != NULL)
  {
    snprintf(username, sizeof(username), "%s", p->pw_name);
    snprintf(configpath, sizeof(configpath),"/home/%s/.config/powerupp", username);
    struct stat st = {0};
    if (stat(configpath, &st) == -1) {
      // config folder doesn't exist, create it
      if(mkdir(configpath, 0755) == -1){
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error creating config folder", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
      }
    }
  }
  else {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "User error", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
  }

  // if upp file is placed in ~/.local/bin and ~/.local/bin doesn't exist in path (old Ubuntu/Debian bug), add it to path
  snprintf(localpath, sizeof(localpath), "/home/%s/.local/bin", username);
  snprintf(localupppath, sizeof(localupppath), "%s/upp", localpath);
  if (access(localupppath, F_OK) != -1) {    
    char *currenv = strdup(getenv("PATH"));
    if(strstr(currenv, localpath) == NULL) {
      char newenv[2048];
      snprintf(newenv, sizeof(newenv),"%s:%s", currenv, localpath);
      setenv("PATH", newenv, 1);
    }
    free(currenv);
  }
}

void get_upp_path(app_widgets *widgets) {
  FILE *fupppath = popen("which upp", "r");
    if (fupppath) {
      if (fgets(upppath, sizeof(upppath), fupppath)){
        size_t len = strlen(upppath);
        if (len > 0 && upppath[len-1] == '\n') {
          upppath[--len] = '\0';
        }
        printf("UPP path is %s\n", upppath);
      }
      else {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "UPP module not found, install with pip", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
      }
    }
    pclose(fupppath);
}

void scan_gpus() {
  //TODO: add more models
    char gpumodel[128];
    char navi10[128] = "12\n";
    char bignavi[128] = "15\n";
    char bignavi2[128] = "18\n";
    int num = 0;
    char cardpath[128];
    char revtable[512];
    char vendorintel[32] = "0x8086\n";
    char vendoramd[32] = "0x1002\n";
    char vendornvidia[32] = "0x10DE\n";
    char vendorcheck[64];
    char vendorid[64];

    //scan for GPUs and add them to combobox
    do {
        snprintf(cardpath, sizeof(cardpath), "/sys/class/drm/card%d/device/device", num);
        snprintf(revtable, sizeof(revtable), "%s --pp-file /sys/class/drm/card%d/device/pp_table get header/format_revision", upppath, num);
        snprintf(vendorcheck, sizeof(vendorcheck),"/sys/class/drm/card%d/device/vendor", num);

        if (access(cardpath, F_OK) != -1) {
        printf("GPU %s exists\n", cardpath);

        FILE *fvendor = fopen(vendorcheck, "r");
        if (fgets(vendorid, sizeof vendorid, fvendor)){
          if (strcmp(vendorid,vendoramd) == 0) {
            FILE *fmodel = popen(revtable, "r");
            if (fgets(gpumodel, sizeof gpumodel, fmodel)){
              printf("GPU %d table revision is %s", num, gpumodel);
              if (strcmp(gpumodel,navi10) == 0) {
                char hgpumodel [128];
                snprintf(hgpumodel, sizeof(hgpumodel), "card %d: AMD Radeon 5xxx (Navi 10)", num);
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(g_combobox), hgpumodel);
                gtk_combo_box_set_active(GTK_COMBO_BOX(g_combobox), num);
              }
              else if (strcmp(gpumodel,bignavi) == 0) || (strcmp(gpumodel,bignavi2) == 0) {
                char hgpumodel [128];
                snprintf(hgpumodel, sizeof(hgpumodel), "card %d: AMD Radeon 6xxx (Big Navi)", num);
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(g_combobox), hgpumodel);
                gtk_combo_box_set_active(GTK_COMBO_BOX(g_combobox), num);
              }
              else {
                char hgpumodel [128];
                snprintf(hgpumodel, sizeof(hgpumodel), "card %d: Unsupported AMD GPU", num);
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(g_combobox), hgpumodel);
                gtk_combo_box_set_active(GTK_COMBO_BOX(g_combobox), num);
              }
            }
            pclose(fmodel);
          }
          else if (strcmp(vendorid,vendorintel) == 0) {
            char hgpumodel [128];
            snprintf(hgpumodel, sizeof(hgpumodel), "card %d: Unsupported Intel GPU", num);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(g_combobox), hgpumodel);
            gtk_combo_box_set_active(GTK_COMBO_BOX(g_combobox), num);
          }
          else if (strcmp(vendorid,vendornvidia) == 0) {
            char hgpumodel [128];
            snprintf(hgpumodel, sizeof(hgpumodel), "card %d: Unsupported Nvidia GPU", num);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(g_combobox), hgpumodel);
            gtk_combo_box_set_active(GTK_COMBO_BOX(g_combobox), num);
          }
          else {
            char hgpumodel [128];
            snprintf(hgpumodel, sizeof(hgpumodel), "card %d: Unsupported GPU", num);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(g_combobox), hgpumodel);
            gtk_combo_box_set_active(GTK_COMBO_BOX(g_combobox), num);
          }
        }
        num++;
        fclose(fvendor);
        }
    } while (access(cardpath, F_OK) != -1);
}

int main(int argc, char *argv[])
{
  GtkBuilder      *builder; 
  GtkWidget       *window;
  app_widgets     *widgets = g_slice_new(app_widgets);

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_resource (builder, "/res/window_main.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
  gtk_builder_connect_signals(builder, widgets);

  // needed to not have problems with commas and dots in float values for eg. staticvoltageoffset
  setlocale(LC_NUMERIC, "C");

  // get pointers to the widgets
  // buttons
  g_btn_active = GTK_BUTTON(gtk_builder_get_object(builder, "btn_active"));
  g_btn_apply = GTK_BUTTON(gtk_builder_get_object(builder, "btn_apply"));
  
  // gpu select
  g_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combobox"));
  g_signal_connect (g_combobox, "changed", G_CALLBACK (on_combobox_changed), widgets);

  // spin buttons with their adjustments and toggles
  widgets->g_edit_gfxvolt = GTK_WIDGET(gtk_builder_get_object(builder, "edit_gfxvolt"));
  g_adj_gfxvolt = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_gfxvolt"));

  widgets->g_edit_gfxvoltmin = GTK_WIDGET(gtk_builder_get_object(builder, "edit_gfxvoltmin"));
  g_adj_gfxvoltmin = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_gfxvoltmin"));

  widgets->g_edit_gfxclock = GTK_WIDGET(gtk_builder_get_object(builder, "edit_gfxclock"));
  g_adj_gfxclock = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_gfxclock"));

  widgets->g_edit_gpupower = GTK_WIDGET(gtk_builder_get_object(builder, "edit_gpupower"));
  g_adj_gpupower = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_gpupower"));

  widgets->g_edit_memmvddvolt = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memmvddvolt"));
  g_adj_memmvddvolt = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memmvddvolt"));

  widgets->g_edit_memvddcivolt = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memvddcivolt"));
  g_adj_memvddcivolt = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memvddcivolt"));

  widgets->g_edit_memclock = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memclock"));
  g_adj_memclock = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memclock"));

  widgets->g_edit_socvolt = GTK_WIDGET(gtk_builder_get_object(builder, "edit_socvolt"));
  g_adj_socvolt = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_socvolt"));

  widgets->g_edit_socvoltmin = GTK_WIDGET(gtk_builder_get_object(builder, "edit_socvoltmin"));
  g_adj_socvoltmin = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_socvoltmin"));

  widgets->g_edit_socclock = GTK_WIDGET(gtk_builder_get_object(builder, "edit_socclock"));
  g_adj_socclock = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_socclock"));

  widgets->g_edit_voltoffset = GTK_WIDGET(gtk_builder_get_object(builder, "edit_voltoffset"));
  g_adj_voltoffset = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_voltoffset"));

  widgets->g_edit_memmvddvolt0 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memmvddvolt0"));
  g_adj_memmvddvolt0 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memmvddvolt0"));

  widgets->g_edit_memvddcivolt0 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memvddcivolt0"));
  g_adj_memvddcivolt0 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memvddcivolt0"));

  widgets->g_edit_memclock0 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memclock0"));
  g_adj_memclock0 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memclock0"));

  widgets->g_edit_memmvddvolt1 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memmvddvolt1"));
  g_adj_memmvddvolt1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memmvddvolt1"));

  widgets->g_edit_memvddcivolt1 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memvddcivolt1"));
  g_adj_memvddcivolt1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memvddcivolt1"));

  widgets->g_edit_memclock1 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memclock1"));
  g_adj_memclock1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memclock1"));

  widgets->g_edit_memmvddvolt2 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memmvddvolt2"));
  g_adj_memmvddvolt2 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memmvddvolt2"));

  widgets->g_edit_memvddcivolt2 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memvddcivolt2"));
  g_adj_memvddcivolt2 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memvddcivolt2"));

  widgets->g_edit_memclock2 = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memclock2"));
  g_adj_memclock2 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memclock2"));

  widgets->g_toggle_limits = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_limits"));

  // info revealer
  widgets->g_revealer = GTK_WIDGET(gtk_builder_get_object(builder, "revealer"));
  g_text_revealer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "text_revealer"));
  widgets->g_textview_revealer = GTK_WIDGET(gtk_builder_get_object(builder, "textview_revealer"));

  // menu options
  widgets->g_opt_profile_load = GTK_WIDGET(gtk_builder_get_object(builder, "opt_profile_load"));
  widgets->g_opt_profile_save = GTK_WIDGET(gtk_builder_get_object(builder, "opt_profile_save"));
  widgets->g_opt_defaults_load = GTK_WIDGET(gtk_builder_get_object(builder, "opt_defaults_load"));
  widgets->g_opt_persistent_save = GTK_WIDGET(gtk_builder_get_object(builder, "opt_persistent_save"));
  widgets->g_opt_persistent_del = GTK_WIDGET(gtk_builder_get_object(builder, "opt_persistent_del"));

  // monitoring fields
  g_lblcurgfxvolt = GTK_LABEL(gtk_builder_get_object(builder, "lbl_curgfxvolt"));
  g_lblcurgfxclock = GTK_LABEL(gtk_builder_get_object(builder, "lbl_curgfxclock"));
  g_lblcurgpupower = GTK_LABEL(gtk_builder_get_object(builder, "lbl_curgpupower"));
  g_lblcursocclock = GTK_LABEL(gtk_builder_get_object(builder, "lbl_cursocclock"));
  g_lblcuredgetemp = GTK_LABEL(gtk_builder_get_object(builder, "lbl_curedgetemp"));
  g_lblcurjunctemp = GTK_LABEL(gtk_builder_get_object(builder, "lbl_curjunctemp"));
  g_lblcurmemtemp = GTK_LABEL(gtk_builder_get_object(builder, "lbl_curmemtemp"));
  g_lblcurfanspeed = GTK_LABEL(gtk_builder_get_object(builder, "lbl_curfanspeed"));
  g_lblcurmemclock = GTK_LABEL(gtk_builder_get_object(builder, "lbl_curmemclock"));

  g_lvlgpuload = GTK_LEVEL_BAR(gtk_builder_get_object(builder, "lvl_gpuload"));
  g_lvlmemload = GTK_LEVEL_BAR(gtk_builder_get_object(builder, "lvl_memload"));

  g_object_unref(builder);
  gtk_widget_show(window);   

  // not used here, but can be accessed from several buttons, needs to be set up at start
  get_home_path(widgets);

  if (strlen(username) != 0) {
    get_upp_path(widgets);	
  }

  if (strlen(upppath) != 0) {
    scan_gpus();
  }
    
  gtk_main();
  g_slice_free(app_widgets, widgets);
  return 0;
}

void on_window_main_destroy()
{
  gtk_main_quit();
}
