void on_btn_apply_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  // TODO: check if set values are ok somehow
  // * 4 for quadruple voltage values
  int igfxvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt)) * 4;
  int igfxvoltmin = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvoltmin)) * 4;
  int igpupower = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower));
  int igfxclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock));
  int imemmvddvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt)) * 4;
  int imemvddcivolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt)) * 4;
  int imemclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock));
  int isocvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt)) * 4;
  int isocvoltmin = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvoltmin)) * 4;
  int isocclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock));
  float fvoltoffset = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_voltoffset)) / 1000;

  int imemmvddvolt0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt0)) * 4;
  int imemvddcivolt0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt0)) * 4;
  int imemclock0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock0));

  int imemmvddvolt1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt1)) * 4;
  int imemvddcivolt1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt1)) * 4;
  int imemclock1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock1));

  int imemmvddvolt2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt2)) * 4;
  int imemvddcivolt2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt2)) * 4;
  int imemclock2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock2));

  char response[2048];
  char applysetup[8192];
  char powerhwmonwrite[512];
  char writecmd[4096];
  
  snprintf(powerhwmonwrite, sizeof(powerhwmonwrite), "echo %d | sudo tee /sys/class/hwmon/$(ls -1 /sys/class/drm/card%d/device/hwmon)/power1_cap", igpupower * 1000000, card_num);

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

  snprintf(applysetup, sizeof applysetup, "pkexec bash -c \"chmod 646 /sys/class/drm/card%d/device/pp_table;sudo -u %s %s;chmod 644 /sys/class/drm/card%d/device/pp_table;%s\"", card_num, p->pw_name, writecmd, card_num, powerhwmonwrite);

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