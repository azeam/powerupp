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

int readcurval(char *rcmd) {
  int val;
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
  int val;
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
      snprintf(uppwrite, sizeof(uppwrite), "%s -m upp -i /sys/class/drm/card%d/device/pp_table set --write", pythonpath, card_num);
     
      snprintf(savedudevpath, sizeof(savedudevpath), "/etc/udev/rules.d/80-powerupp%d.rules", card_num);
      snprintf(hwmonprepath, sizeof(hwmonprepath), "/sys/class/drm/card%d/device/hwmon", card_num);
      snprintf(uppdump, sizeof(uppdump), "%s -m upp -i /sys/class/drm/card%d/device/pp_table dump > %s", pythonpath, card_num, ftempname);
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

  // remove tempfile if present after changing
  if(access(ftempname, F_OK) != -1) {
    if (remove(ftempname) == 0) {
      printf("Cleaned up leftover temp file %s successfully\n", ftempname); 
    }
  }
  char template[512];
  snprintf(template, sizeof template, "%s/poweruppXXXXXX", tempdirectory);
  strcpy(ftempname, template);		
  mkstemp(ftempname);

  if (gtk_combo_box_text_get_active_text (g_combobox) != 0) {
    card_num = gtk_combo_box_get_active(GTK_COMBO_BOX(g_combobox));
    gchar *card_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_combobox));
    char navi10[512];
    snprintf(navi10, sizeof(navi10), "card %d: AMD Radeon 5xxx (Navi 10)", card_num);
    if (strcmp(card_text,navi10) == 0) {
      // data specific for navi 10, possible to add data for other GPUs
      // get values, using regex from tmp file is hacky and takes away from the functionality of UPP but is necessary to speed up the calls
      snprintf(pp_gfxvolt, sizeof(pp_gfxvolt), "cat %s | grep MaxVoltageGfx | cut -c18- | tr -d $'\n'", ftempname);
      snprintf(pp_gfxvoltmin, sizeof(pp_gfxvoltmin), "cat %s | grep MinVoltageGfx | cut -c18- | tr -d $'\n'", ftempname);
      snprintf(pp_gfxclock, sizeof(pp_gfxclock), "cat %s | grep 'FreqTableGfx 1:' | cut -c21- | tr -d $'\n'", ftempname);
      snprintf(pp_gpupower, sizeof(pp_gpupower), "cat %s | grep 'LimitAc 0:' | cut -c27- | tr -d $'\n'", ftempname);
      snprintf(pp_memmvddvolt, sizeof(pp_memmvddvolt), "cat %s | grep 'MemMvddVoltage 3:' | cut -c23- | tr -d $'\n'", ftempname);
      snprintf(pp_memvddcivolt, sizeof(pp_memvddcivolt), "cat %s | grep 'MemVddciVoltage 3:' | cut -c24- | tr -d $'\n'", ftempname);
      snprintf(pp_memclock, sizeof(pp_memclock), "cat %s | grep 'FreqTableUclk 3:' | cut -c22- | tr -d $'\n'", ftempname);
      snprintf(pp_socvoltmin, sizeof(pp_socvoltmin), "cat %s | grep MinVoltageSoc | cut -c18- | tr -d $'\n'", ftempname);
      snprintf(pp_socvolt, sizeof(pp_socvolt), "cat %s | grep MaxVoltageSoc | cut -c18- | tr -d $'\n'", ftempname);
      snprintf(pp_socclock, sizeof(pp_socclock), "cat %s | grep 'FreqTableSocclk 1:' | cut -c24- | tr -d $'\n'", ftempname);
      snprintf(pp_voltoffset, sizeof(pp_voltoffset), "cat %s | awk 'c&&!--c;/qStaticVoltageOffset 0:/{c=3}' | cut -c10- | tr -d $'\n'", ftempname); 
      snprintf(pp_memmvddvolt0, sizeof(pp_memmvddvolt0), "cat %s | grep 'MemMvddVoltage 0:' | cut -c23- | tr -d $'\n'", ftempname);
      snprintf(pp_memvddcivolt0, sizeof(pp_memvddcivolt0), "cat %s | grep 'MemVddciVoltage 0:' | cut -c24- | tr -d $'\n'", ftempname);
      snprintf(pp_memclock0, sizeof(pp_memclock0), "cat %s | grep 'FreqTableUclk 0:' | cut -c22- | tr -d $'\n'", ftempname);
      snprintf(pp_memmvddvolt1, sizeof(pp_memmvddvolt1), "cat %s | grep 'MemMvddVoltage 1:' | cut -c23- | tr -d $'\n'", ftempname);
      snprintf(pp_memvddcivolt1, sizeof(pp_memvddcivolt1), "cat %s | grep 'MemVddciVoltage 1:' | cut -c24- | tr -d $'\n'", ftempname);
      snprintf(pp_memclock1, sizeof(pp_memclock1), "cat %s | grep 'FreqTableUclk 1:' | cut -c22- | tr -d $'\n'", ftempname);
      snprintf(pp_memmvddvolt2, sizeof(pp_memmvddvolt2), "cat %s | grep 'MemMvddVoltage 2:' | cut -c23- | tr -d $'\n'", ftempname);
      snprintf(pp_memvddcivolt2, sizeof(pp_memvddcivolt2), "cat %s | grep 'MemVddciVoltage 2:' | cut -c24- | tr -d $'\n'", ftempname);
      snprintf(pp_memclock2, sizeof(pp_memclock2), "cat %s | grep 'FreqTableUclk 2:' | cut -c22- | tr -d $'\n'", ftempname);

      //get limits where available
      snprintf(pp_gfxvoltlimitupper, sizeof(pp_gfxvoltlimitupper), "cat %s | awk 'c&&!--c;/overdrive_table/{c=49}' | cut -c12- | tr -d $'\n'", ftempname);
      snprintf(pp_gfxclocklimitupper, sizeof(pp_gfxclocklimitupper), "cat %s | awk 'c&&!--c;/overdrive_table/{c=42}' | cut -c12- | tr -d $'\n'", ftempname);
      snprintf(pp_gpupowerlimitupper, sizeof(pp_gpupowerlimitupper), "cat %s | awk 'c&&!--c;/overdrive_table/{c=51}' | cut -c12- | tr -d $'\n'", ftempname);
      snprintf(pp_memclocklimitupper, sizeof(pp_memclocklimitupper), "cat %s | awk 'c&&!--c;/overdrive_table/{c=50}' | cut -c12- | tr -d $'\n'", ftempname);       

      snprintf(pp_gfxvoltlimitlower, sizeof(pp_gfxvoltlimitlower), "cat %s | awk 'c&&!--c;/overdrive_table/{c=82}' | cut -c12- | tr -d $'\n'", ftempname);
      snprintf(pp_gfxclocklimitlower, sizeof(pp_gfxclocklimitlower), "cat %s | awk 'c&&!--c;/overdrive_table/{c=75}' | cut -c12- | tr -d $'\n'", ftempname);
      snprintf(pp_gpupowerlimitlower, sizeof(pp_gpupowerlimitlower), "cat %s | awk 'c&&!--c;/overdrive_table/{c=84}' | cut -c12- | tr -d $'\n'", ftempname);
      snprintf(pp_memclocklimitlower, sizeof(pp_memclocklimitlower), "cat %s | awk 'c&&!--c;/overdrive_table/{c=83}' | cut -c12- | tr -d $'\n'", ftempname);

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

      gl_revtable = 12;

      setup_gpu_paths_and_options(app_wdgts);
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

void get_temp_path(app_widgets *widgets) {
  tempdirectory = getenv("XDG_RUNTIME_DIR");
  printf("Environment variable XDG_RUNTIME_DIR: %s\n", tempdirectory);
  if (tempdirectory == NULL) {
    tempdirectory = getenv("TMPDIR");
    printf("Environment variable TMPDIR: %s\n", tempdirectory);
  }
  if (tempdirectory == NULL) {
    tempdirectory = "/tmp";
    printf("No environment variables XDG_RUNTIME_DIR or TMPDIR set, defaulting to: %s\n", tempdirectory);
  }
  if (tempdirectory != NULL) {
    if(access(tempdirectory, W_OK) == 0) {
      printf("Using %s as temp folder\n", tempdirectory);
    }
    else {
      gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error, no tmp folder write access", -1);
      gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
    }
  }
}

void get_home_path(app_widgets *widgets) {
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
}

void get_python_path(app_widgets *widgets) {
  FILE *fpython3path = popen("which python3", "r");
    if (fpython3path) {
      if (fgets(pythonpath, sizeof(pythonpath), fpython3path)){
        size_t len = strlen(pythonpath);
        if (len > 0 && pythonpath[len-1] == '\n') {
          pythonpath[--len] = '\0';
        }
        printf("Python path used is %s\n", pythonpath);
      }
      else {
        printf("Python3 path not found, checking for Python2\n");
        FILE *fpythonpath = popen("which python", "r");
        if (fpythonpath) {
          if (fgets(pythonpath, sizeof(pythonpath), fpythonpath)){
            size_t len = strlen(pythonpath);
            if (len > 0 && pythonpath[len-1] == '\n') {
              pythonpath[--len] = '\0';
            }
            printf("Python path used is %s\n", pythonpath);
          }
          else {
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error, path to Python could not be found", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
          }
        }
        pclose(fpythonpath);
      }
    }
    pclose(fpython3path);
}

int test_upp(app_widgets *widgets) {
  char ctestupp[1024];
  char testupp[512];
  char ftemptestname[256];
  char template[256];
  int upperror = 0;
  snprintf(template, sizeof template, "%s/poweruppXXXXXX", tempdirectory);
  strcpy(ftemptestname, template);		
  mkstemp(ftemptestname);
  snprintf (ctestupp, sizeof ctestupp, "%s -m upp > %s 2>&1", pythonpath, ftemptestname);
  // test if UPP seems to work as it should, works but ugly workaround for not being able to read the output from python error with fgets
  FILE *ftestupptmp = popen(ctestupp, "r");
  pclose(ftestupptmp);
  
  if (access(ftemptestname, F_OK) != -1) {
    FILE *ftestupp = fopen(ftemptestname, "r");
    if(ftestupp != NULL) {
      while(fgets(testupp, sizeof testupp, ftestupp)){
        if(strstr(testupp,"ModuleNotFoundError") != NULL) {
          printf("UPP: %s", testupp);
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "UPP error, missing dependencies", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
          upperror = 1;
          break;
        }
        else if (strstr(testupp,"not found") != NULL){
          printf("UPP: %s", testupp);
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "UPP error, file not found", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
          upperror = 1;
          break;
        }
        else if (strstr(testupp,"No module named upp") != NULL){
          printf("UPP: %s", testupp);
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "UPP module not found, install with pip", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
          upperror = 1;
          break;
        }
      }
    }
    fclose(ftestupp);
  }
 
  if (remove(ftemptestname) == 0) {
    printf("Temp upptest file %s deleted successfully\n", ftemptestname); 
  }
  else {
    printf("Unable to delete the temp upptest file\n");
  }
  if (upperror == 1) {
    return 1;
  }
  else {
    return 0;
  }
}

void scan_gpus() {
  //TODO: add more models
    char gpumodel[128];
    char navi10[128] = "12\n";
    int num = 0;
    char cardnum[128];
    char revtable[512];
    char vendorintel[32] = "0x8086\n";
    char vendoramd[32] = "0x1002\n";
    char vendornvidia[32] = "0x10DE\n";
    char vendorcheck[64];
    char vendorid[64];

    //scan for GPUs and add them to combobox
    do {
        snprintf(cardnum, sizeof(cardnum), "/sys/class/drm/card%d/device/device", num);
        snprintf(revtable, sizeof(revtable), "%s -m upp -i /sys/class/drm/card%d/device/pp_table get header/format_revision", pythonpath, num);
        snprintf(vendorcheck, sizeof(vendorcheck),"/sys/class/drm/card%d/device/vendor", num);

        if (access(cardnum, F_OK) != -1) {
        printf("GPU %s exists\n", cardnum);

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
    } while (access(cardnum, F_OK) != -1);
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

  // needs to be set before testing upp
  g_object_unref(builder);
  gtk_widget_show(window);   

  // not used here, but can be accessed from several buttons, best to set it up at start
  get_home_path(widgets);

  get_temp_path(widgets);
  if (strlen(tempdirectory) != 0) {
    get_python_path(widgets);	
  }

  if (strlen(pythonpath) != 0) {
    printf("python path %s\n", pythonpath);
    if (test_upp(widgets) == 0) {
      scan_gpus();
    }
  }
    
  gtk_main();
  g_slice_free(app_widgets, widgets);
  return 0;
}

void on_window_main_destroy()
{
  //remove temp file if load active has not been run
  if(access(ftempname, F_OK) != -1) {
    if (remove(ftempname) == 0) {
      printf("Cleaned up leftover temp file %s successfully\n", ftempname); 
    }
  }
  gtk_main_quit();
}
