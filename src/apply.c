#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "variables.h"

void on_btn_apply_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  char response[2048];
  char applysetup[8192];
  char powersysfswrite[512];
  char writecmd[4096];
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

  // set writecmd dependent on pp table revision number, this way cards with less settings can be added
  // * 4 for quadruple voltage values
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

    snprintf(powersysfswrite, sizeof(powersysfswrite), "echo %d | sudo tee /sys/class/hwmon/$(ls -1 /sys/class/drm/card%d/device/hwmon)/power1_cap", igpupower * 1000000, card_num);

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
    snprintf(applysetup, sizeof applysetup, "pkexec bash -c \"chmod 666 /sys/class/drm/card%d/device/pp_table;sudo -i -u %s %s %s;chmod 644 /sys/class/drm/card%d/device/pp_table;%s\"", card_num, username, upppath, writecmd, card_num, powersysfswrite);

    //reset pp table chmod to 644 for safety, password has to be written every time though
    //TODO: add option to disable pkexec promt and keep chmod 646?
    FILE *frunapply = popen(applysetup, "r");
    if (frunapply != NULL) {
      while(fgets(response, sizeof response, frunapply)){
        printf("%s\n", response);
      }
      int status = pclose(frunapply);
      if (WEXITSTATUS(status) == 0) {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Settings successfully applied", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
      }
      else if (WEXITSTATUS(status) == 126) {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Dialog cancelled, nothing applied", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
      }      
    }
  }
  else {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error, nothing applied", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
  } 
}