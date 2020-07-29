#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "variables.h" 

void on_opt_persistent_del_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts) {
  char defudevpath[128] = "/etc/udev/rules.d/";
  char deludevcmd[512];
  snprintf(deludevcmd, sizeof(deludevcmd), "pkexec rm -f %s80-powerupp%d.rules /usr/bin/powerupp_startup_script_card%d.sh", defudevpath, card_num, card_num);
  struct stat st = {0};
    //confirm udev default path is present
    if (stat(defudevpath, &st) == -1) {
      gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Path to udev not found, unable to delete", -1);
      gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
    }
    else {
      FILE *fdeludev = popen (deludevcmd, "r"); 
      if (fdeludev != NULL) {
        int status = pclose(fdeludev);
        if (WEXITSTATUS(status) == 0) {
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Startup scripts for this GPU deleted", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_persistent_del), FALSE);
        }
        else if (WEXITSTATUS(status) == 126) {
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Dialog cancelled, nothing deleted", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
        }
      }
      else {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Unable to delete startup scripts", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE); 
      }
    }
}

void on_opt_persistent_save_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts) {
  
  char powersysfswrite[248]; 
  char scriptpath[248];
  char chmodbashscript[512];
  char writecmd[4096];
  char bashscript[8192];
  char shebang[128] = "#!/bin/bash";
  char udevcmd[16384];
  char prefix[64] = "/usr/bin";
  char cardid[32];
  char cardidpath[128];
  char udevrules[512];
  char udevpath[256];
  char defudevpath[128] = "/etc/udev/rules.d/";

  int readerror = 0;
  int igfxvolt;
  int igfxvoltmin;
  int igpupower;
  int igfxclock;
  int imemmvddvolt;
  int imemvddcivolt;
  int imemclock;
  int isocvolt;
  int isocvoltmin;
  int isocclock;
  float fvoltoffset;
  int imemmvddvolt0;
  int imemvddcivolt0;
  int imemclock0;
  int imemmvddvolt1;
  int imemvddcivolt1;
  int imemclock1;
  int imemmvddvolt2;
  int imemvddcivolt2;
  int imemclock2;

  // TODO: add more GPU families
  if (gl_revtable == 12) {
    
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_gfxvolt)),"N/A") != 0) {
      igfxvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt)) * 4;
    }
    else {
      readerror = 1;
    }
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_gfxvoltmin)),"N/A") != 0) {
      igfxvoltmin = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvoltmin)) * 4;
    }
    else {
      readerror = 1;
    }
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_gpupower)),"N/A") != 0) {
      igpupower = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower));
    }
    else {
      readerror = 1;
    }
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_gfxclock)),"N/A") != 0) {
      igfxclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock));
    }
    else {
      readerror = 1;
    }
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memmvddvolt)),"N/A") != 0) {
      imemmvddvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt)) * 4;
    }
    else {
      readerror = 1;
    } 
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memvddcivolt)),"N/A") != 0) {
      imemvddcivolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt)) * 4;
    }
    else {
      readerror = 1;
    }
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memclock)),"N/A") != 0) {
      imemclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock));
    }
    else {
      readerror = 1;
    }
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_socvolt)),"N/A") != 0) {
      isocvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt)) * 4;
    }
    else {
      readerror = 1;
    }

    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_socvoltmin)),"N/A") != 0) {
      isocvoltmin = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvoltmin)) * 4;
    }
    else {
      readerror = 1;
    }
    
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_socclock)),"N/A") != 0) {
      isocclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock));
    }
    else {
      readerror = 1;
    }
    
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_voltoffset)),"N/A") != 0) {
      fvoltoffset = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_voltoffset)) / 1000;
    }
    else {
      readerror = 1;
    }

    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memmvddvolt0)),"N/A") != 0) {
      imemmvddvolt0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt0)) * 4;
    }
    else {
      readerror = 1;
    }
    
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memvddcivolt0)),"N/A") != 0) {
      imemvddcivolt0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt0)) * 4;
    }
    else {
      readerror = 1;
    }

    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memclock0)),"N/A") != 0) {
      imemclock0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock0));
    }
    else {
      readerror = 1;
    }
    
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memmvddvolt1)),"N/A") != 0) {
      imemmvddvolt1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt1)) * 4;
    }
    else {
      readerror = 1;
    }
    
    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memvddcivolt1)),"N/A") != 0) {
      imemvddcivolt1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt1)) * 4;
    }
    else {
      readerror = 1;
    }

    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memclock1)),"N/A") != 0) {
      imemclock1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock1));
    }
    else {
      readerror = 1;
    }

    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memmvddvolt2)),"N/A") != 0) {
      imemmvddvolt2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt2)) * 4;
    }
    else {
      readerror = 1;
    }

    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memvddcivolt2)),"N/A") != 0) {
      imemvddcivolt2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt2)) * 4;
    }
    else {
      readerror = 1;
    }

    if (strcmp(gtk_entry_get_text (GTK_ENTRY (app_wdgts->g_edit_memclock2)),"N/A") != 0) {
      imemclock2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock2));
    }
    else {
      readerror = 1;
    }

    // set up cmd to write to sysfs because power pp table not working. Setting the pp table limit has to be done first though or it will not work, send sysfs cmd after
    snprintf(powersysfswrite, sizeof(powersysfswrite), "echo %d | tee /sys/class/hwmon/$(ls -1 /sys/class/drm/card%d/device/hwmon)/power1_cap", igpupower * 1000000, card_num);
    
    // fill data to bashscript with upp commands
    snprintf(writecmd, sizeof(writecmd), "%s %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%f %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d", uppwrite,\
    gfxvoltset, igfxvolt,\
    gpupowerset, igpupower,\
    gfxclockset, igfxclock,\
    memmvddvoltset, imemmvddvolt,\
    memvddcivoltset, imemvddcivolt,\
    memclockset, imemclock,\
    socvoltset, isocvolt,\
    socclockset, isocclock,\
    voltoffsetset, fvoltoffset,\
    memmvddvoltset0, imemmvddvolt0,\
    memvddcivoltset0, imemvddcivolt0,\
    memclockset0, imemclock0,\
    memmvddvoltset1, imemmvddvolt1,\
    memvddcivoltset1, imemvddcivolt1,\
    memclockset1, imemclock1,\
    memmvddvoltset2, imemmvddvolt2,\
    memvddcivoltset2, imemvddcivolt2,\
    memclockset2, imemclock2,\
    gfxvoltminset, igfxvoltmin,\
    socvoltminset, isocvoltmin\
    );
  }

  if (readerror == 0) {
    snprintf(udevpath, sizeof(udevpath), "%s80-powerupp%d.rules", defudevpath, card_num);
    snprintf(cardidpath, sizeof(cardidpath), "/sys/class/drm/card%d/device/device", card_num);
    // get pci id without newline
    FILE *fcardid = fopen(cardidpath, "r");
      if (fgets(cardid, sizeof(cardid), fcardid)){
        size_t len = strlen(cardid);
        if (len > 0 && cardid[len-1] == '\n') {
          cardid[--len] = '\0';
        }
      }
      else {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Card PCI ID not found, unable to save", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
      }
    fclose(fcardid);
    snprintf(udevrules, sizeof(udevrules), "echo 'KERNEL==\\\"card%d\\\", SUBSYSTEM==\\\"drm\\\", DRIVERS==\\\"amdgpu\\\", ATTRS{device}==\\\"%s\\\", RUN+=\\\"/bin/bash %s/powerupp_startup_script_card%d.sh\\\"' > %s", card_num, cardid, prefix, card_num, udevpath);
    snprintf(scriptpath, sizeof(scriptpath), "%s/powerupp_startup_script_card%d.sh", prefix, card_num);
    snprintf(bashscript, sizeof(bashscript), "echo '%s\nchmod 666 /sys/class/drm/card%d/device/pp_table\nsudo -u %s %s %s %s\nchmod 644 /sys/class/drm/card%d/device/pp_table\n%s' > %s", shebang, card_num, username, "python3", upppath, writecmd, card_num, powersysfswrite, scriptpath);
    snprintf(chmodbashscript, sizeof(chmodbashscript), "chmod 755 %s", scriptpath);
    snprintf(udevcmd, sizeof(udevcmd), "pkexec bash -c \"%s;%s;%s\"", bashscript, chmodbashscript, udevrules);
  
    struct stat st = {0};
    //confirm udev default path is present
    if (stat(defudevpath, &st) == -1) {
      gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Path to udev not found, unable to save", -1);
      gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
    }
    else {
      FILE *writeudev = popen (udevcmd, "r"); 
      if (writeudev != NULL) {
        int status = pclose(writeudev);
        if (WEXITSTATUS(status) == 0) {
          //confirm file was actually written
          if (stat(udevpath, &st) == -1) {
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error saving udev rules", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
          }
          else {
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Settings successfully saved", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_persistent_del), TRUE);
          }
        }
        else if (WEXITSTATUS(status) == 126) {
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Dialog cancelled, nothing saved", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
        }
      }
      else {
        printf("Unable to create udev file\n");
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Unable to create udev file", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE); 
      }
    }
  }
  else {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error, nothing saved", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
  }
}