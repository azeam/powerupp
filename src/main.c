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
 
  char curgfxvolt[600];
  snprintf(curgfxvolt, sizeof(curgfxvolt), "cat %s/in0_input | tr -d $'\n' 2>/dev/null", hwmonpath);
  char curgfxclock[600];
  snprintf(curgfxclock, sizeof(curgfxclock), "cat %s/freq1_input | tr -d $'\n' 2>/dev/null", hwmonpath);
  char curgpupower[600];
  snprintf(curgpupower, sizeof(curgpupower), "cat %s/power1_average | tr -d $'\n' 2>/dev/null", hwmonpath);
  char cursocclock[600];
  snprintf(cursocclock, sizeof(cursocclock), "cat /sys/class/drm/card%d/device/pp_dpm_socclk | grep '*' | cut -d' ' -f2 | tr -dc '0-9'", card_num);
  char curedgetemp[600];
  snprintf(curedgetemp, sizeof(curedgetemp), "cat %s/temp1_input | tr -d $'\n' 2>/dev/null", hwmonpath);
  char curjunctemp[600];
  snprintf(curjunctemp, sizeof(curjunctemp), "cat %s/temp2_input | tr -d $'\n' 2>/dev/null", hwmonpath);
  char curmemtemp[600];
  snprintf(curmemtemp, sizeof(curmemtemp), "cat %s/temp3_input | tr -d $'\n' 2>/dev/null", hwmonpath);
  char curfanspeed[600];
  snprintf(curfanspeed, sizeof(curfanspeed), "cat %s/fan1_input | tr -d $'\n' 2>/dev/null", hwmonpath);
  char curmemclock[600];
  snprintf(curmemclock, sizeof(curmemclock), "cat %s/freq2_input | tr -d $'\n' 2>/dev/null", hwmonpath);
  char curgpuload[600];
  snprintf(curgpuload, sizeof(curgpuload), "cat /sys/class/drm/card%d/device/gpu_busy_percent | tr -dc '0-9'", card_num);
  char curmemload[600];
  snprintf(curmemload, sizeof(curmemload), "cat /sys/class/drm/card%d/device/mem_busy_percent | tr -dc '0-9'", card_num);

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

// lost patience with glade, connecting manually
static void on_combobox_changed (GtkComboBoxText *combobox, gpointer user_data) {
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_active), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_defaults), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_perm), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), FALSE);

    char template[512];
    snprintf(template, sizeof template, "%s/poweruppXXXXXX", tempdirectory);
    strcpy(ftempname, template);		
    mkstemp(ftempname);

    if (gtk_combo_box_text_get_active_text (g_combobox) != 0) {
      card_num = gtk_combo_box_get_active(GTK_COMBO_BOX(g_combobox));
      gchar *card_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_combobox));
      char navi10[512];
      char hwmonprepath[256];
      snprintf(navi10, sizeof(navi10), "card %d: AMD Radeon 5xxx (Navi 10)", card_num);
      if (strcmp(card_text,navi10) == 0) {
            snprintf(hwmonprepath, sizeof(hwmonprepath), "/sys/class/drm/card%d/device/hwmon", card_num);

            // hwmon subfolder can contain any number in its name, search for subfolders and use the first folder containing hwmon as monitoring path, can't use ls in path when checking for access
            DIR *dp;
            struct dirent *entry;
            struct stat statbuf;
            char cwd[512];
            if((dp = opendir(hwmonprepath)) != NULL) {
              chdir(hwmonprepath);
              while((entry = readdir(dp)) != NULL) {
                lstat(entry->d_name,&statbuf);
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
                return; 
            }
            closedir(dp);

            // start monitoring
            if(access(hwmonpath, R_OK ) == 0) {
              g_timeout_add (200 /* milliseconds */, monitoring, NULL);
            }
            else {
              printf("Can't access %s, unable to monitor\n", hwmonpath);
            }

            snprintf(uppdump, sizeof(uppdump), "upp.py -i /sys/class/drm/card%d/device/pp_table dump > %s", card_num, ftempname);

            // data for 5700 XT, possible to add data for other GPUs
            // get values, using regex is ugly and takes away from the functionality of UPP but really helps to speed up the calls
            snprintf(gfxvolt, sizeof(gfxvolt), "cat %s | grep MaxVoltageGfx | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(gfxvoltmin, sizeof(gfxvoltmin), "cat %s | grep MinVoltageGfx | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(gfxclock, sizeof(gfxclock), "cat %s | awk 'c&&!--c;/FreqTableGfx/{c=2}' | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(gpupower, sizeof(gpupower), "cat %s | awk 'c&&!--c;/SocketPowerLimitAc/{c=1}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memmvddvolt, sizeof(memmvddvolt), "cat %s | awk 'c&&!--c;/MemMvddVoltage/{c=4}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memvddcivolt, sizeof(memvddcivolt), "cat %s | awk 'c&&!--c;/MemVddciVoltage/{c=4}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memclock, sizeof(memclock), "cat %s | awk 'c&&!--c;/FreqTableUclk/{c=4}' | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(socvoltmin, sizeof(socvoltmin), "cat %s | grep MinVoltageSoc | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(socvolt, sizeof(socvolt), "cat %s | grep MaxVoltageSoc | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(socclock, sizeof(socclock), "cat %s | awk 'c&&!--c;/FreqTableSocclk/{c=2}' | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(voltoffset, sizeof(voltoffset), "cat %s | awk 'c&&!--c;/qStaticVoltageOffset 0/{c=3}' | cut -c10- | tr -d $'\n'", ftempname);
            snprintf(memmvddvolt0, sizeof(memmvddvolt0), "cat %s | awk 'c&&!--c;/MemMvddVoltage/{c=1}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memvddcivolt0, sizeof(memvddcivolt0), "cat %s | awk 'c&&!--c;/MemVddciVoltage/{c=1}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memclock0, sizeof(memclock0), "cat %s | awk 'c&&!--c;/FreqTableUclk/{c=1}' | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(memmvddvolt1, sizeof(memmvddvolt1), "cat %s | awk 'c&&!--c;/MemMvddVoltage/{c=2}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memvddcivolt1, sizeof(memvddcivolt1), "cat %s | awk 'c&&!--c;/MemVddciVoltage/{c=2}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memclock1, sizeof(memclock1), "cat %s | awk 'c&&!--c;/FreqTableUclk/{c=2}' | cut -c18- | tr -d $'\n'", ftempname);
            snprintf(memmvddvolt2, sizeof(memmvddvolt2), "cat %s | awk 'c&&!--c;/MemMvddVoltage/{c=3}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memvddcivolt2, sizeof(memvddcivolt2), "cat %s | awk 'c&&!--c;/MemVddciVoltage/{c=3}' | cut -c16- | tr -d $'\n'", ftempname);
            snprintf(memclock2, sizeof(memclock2), "cat %s | awk 'c&&!--c;/FreqTableUclk/{c=3}' | cut -c18- | tr -d $'\n'", ftempname);

            //get limits where available
            snprintf(gfxvoltlimits, sizeof(gfxvoltlimits), "cat %s | awk 'c&&!--c;/ODSettingsMax/{c=4}' | cut -c16- && cat %s | awk 'c&&!--c;/ODSettingsMin/{c=4}' | cut -c16- | tr -d $'\n'", ftempname, ftempname);
            snprintf(gfxclocklimits, sizeof(gfxclocklimits), "cat %s | awk 'c&&!--c;/ODSettingsMax/{c=2}' | cut -c16- && cat %s | awk 'c&&!--c;/ODSettingsMin/{c=2}' | cut -c16- | tr -d $'\n'", ftempname, ftempname);
            snprintf(gpupowerlimits, sizeof(gpupowerlimits), "cat %s | awk 'c&&!--c;/ODSettingsMax/{c=10}' | cut -c16- && cat %s | awk 'c&&!--c;/ODSettingsMin/{c=10}' | cut -c16- | tr -d $'\n'", ftempname, ftempname);
            snprintf(memclocklimits, sizeof(memclocklimits), "cat %s | awk 'c&&!--c;/ODSettingsMax/{c=9}' | cut -c16- && cat %s | awk 'c&&!--c;/ODSettingsMin/{c=9}' | cut -c16- | tr -d $'\n'", ftempname, ftempname);            

            //set up write commands
            //set "--write" at the end of cmd after testing or remove for testing
            snprintf(uppwrite, sizeof(uppwrite), "upp.py -i /sys/class/drm/card%d/device/pp_table set --write", card_num);
            gfxvoltset = "smcPPTable/MaxVoltageGfx=";
            gfxvoltminset = "smcPPTable/MinVoltageGfx=";
            gpupowerset = "smcPPTable/SocketPowerLimitAc/0=";
            gfxclockset = "smcPPTable/FreqTableGfx/1=";
            memmvddvoltset = "smcPPTable/MemMvddVoltage/3=";
            memvddcivoltset = "smcPPTable/MemVddciVoltage/3=";
            memclockset = "smcPPTable/FreqTableUclk/3=";
            socvoltset = "smcPPTable/MaxVoltageSoc=";
            socvoltminset = "smcPPTable/MinVoltageSoc=";
            socclockset = "smcPPTable/FreqTableSocclk/1=";
            voltoffsetset = "smcPPTable/qStaticVoltageOffset/0/c=";
            memmvddvoltset0 = "smcPPTable/MemMvddVoltage/0=";
            memvddcivoltset0 = "smcPPTable/MemVddciVoltage/0=";
            memclockset0 = "smcPPTable/FreqTableUclk/0=";
            memmvddvoltset1 = "smcPPTable/MemMvddVoltage/1=";
            memvddcivoltset1 = "smcPPTable/MemVddciVoltage/1=";
            memclockset1 = "smcPPTable/FreqTableUclk/1=";
            memmvddvoltset2 = "smcPPTable/MemMvddVoltage/2=";
            memvddcivoltset2 = "smcPPTable/MemVddciVoltage/2=";
            memclockset2 = "smcPPTable/FreqTableUclk/2=";

            //get user home directory
            p = getpwuid(getuid());
            if (p != NULL)
            {
              snprintf(dirname, sizeof(dirname),"/home/%s/.config/powerupp", p->pw_name);
              snprintf(valuespath, sizeof(valuespath), "%s/defaultvalues%d", dirname, card_num);
              snprintf(limitspath, sizeof(limitspath), "%s/defaultlimits%d", dirname, card_num);
            }
            else {
              printf("User error\n");
            }

            gtk_widget_set_sensitive(GTK_WIDGET(g_btn_active), TRUE);
                if(access(valuespath, F_OK ) != -1 && access(limitspath, F_OK ) != -1) {
                  gtk_widget_set_sensitive(GTK_WIDGET(g_btn_defaults), TRUE);
                }
      }
      g_free(card_text);
    }
    
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
    g_btn_defaults = GTK_BUTTON(gtk_builder_get_object(builder, "btn_defaults"));
    g_btn_apply = GTK_BUTTON(gtk_builder_get_object(builder, "btn_apply"));
    g_btn_perm = GTK_BUTTON(gtk_builder_get_object(builder, "btn_perm"));
    
    // gpu select
    g_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combobox"));
    g_signal_connect (g_combobox, "changed", G_CALLBACK (on_combobox_changed), NULL);

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

    widgets->g_revealer = GTK_WIDGET(gtk_builder_get_object(builder, "revealer"));
    g_text_revealer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "text_revealer"));
    widgets->g_textview_revealer = GTK_WIDGET(gtk_builder_get_object(builder, "textview_revealer"));

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

    int error = 0;
    
    //set up tmp directory and confirm write access
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
        error = 1;
      }
    }

    char template[256];
    snprintf(template, sizeof template, "%s/poweruppXXXXXX", tempdirectory);
    char ftemptestname[256];

    strcpy(ftemptestname, template);		
    mkstemp(ftemptestname);		

    if (error == 0) {
      char ctestupp[1024];
      char cattestupp[2048];
      snprintf (ctestupp, sizeof ctestupp, "upp.py > %s 2>&1", ftemptestname);
      snprintf (cattestupp, sizeof cattestupp, "cat %s", ftemptestname);
      // test if UPP seems to work as it should, works but ugly workaround for not being able to read the output from python error with fgets
      FILE *ftestupptmp = popen(ctestupp, "r");
      pclose(ftestupptmp);
      FILE *ftestupp = popen(cattestupp, "r");
      char testupp[512];
      int upperror = 0;
      if(ftestupp != NULL) {
        while(fgets(testupp, sizeof testupp, ftestupp)){
        if(strstr(testupp,"ModuleNotFoundError") != NULL) {
          printf("UPP %s\n", testupp);
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "UPP error, missing dependencies", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
          upperror = 1;
          break;
        }
        else if (strstr(testupp,"not found") != NULL){
          printf("UPP %s\n", testupp);
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "UPP error, file not found", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
          upperror = 1;
          break;
        }
        }
      }
      pclose(ftestupp);
      
      if (remove(ftemptestname) == 0) {
        printf("Temp upptest file %s deleted successfully\n", ftemptestname); 
      }
      else {
        printf("Unable to delete the temp upptest file\n");
      }

      if (upperror == 0) {
      //TODO: add more models
      char gpumodel[128];
      char navi10[128] = "12\n";
      int num = 0;
      char cardnum[128];
      char revtable[128];
      char vendorintel[32] = "0x8086\n";
      char vendoramd[32] = "0x1002\n";
      char vendornvidia[32] = "0x10DE\n";
      char vendorcheck[64];
      char vendorid[64];

      //scan for GPUs and add them to combobox
      do {
          snprintf(cardnum, sizeof(cardnum), "/sys/class/drm/card%d/device/device", num);
          snprintf(revtable, sizeof(revtable), "upp.py -i /sys/class/drm/card%d/device/pp_table get /TableFormatRevision", num);
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
    }

    gtk_main();

    g_slice_free(app_widgets, widgets);
    return 0;
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
