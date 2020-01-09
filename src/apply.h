void on_btn_apply_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  // TODO: check if set values are ok somehow
  char response[2048];

  char statpath[512];
  snprintf(statpath, sizeof(statpath), "stat -c '%%a %%n' /sys/class/drm/card%d/device/pp_table  | cut -d' ' -f1", card_num);
  FILE *checkroot = popen(statpath, "r");
  if(fgets(response, sizeof response, checkroot)){
    
    if (strcmp(response,"646\n") == 0) {
        printf("Write permission to /sys/class/drm/card%d/device/pp_table OK, no need for root\n", card_num);
    }
    else {
      char chmodpptable[512];
      snprintf(chmodpptable, sizeof(chmodpptable), "pkexec chmod 646 /sys/class/drm/card%d/device/pp_table", card_num);
      FILE *getroot = popen(chmodpptable, "r");
      if(fgets(response, sizeof response, getroot)){
          printf("Cannot write to /sys/class/drm/card%d/device/pp_table, chmod file as root\n%s", card_num, response);  
      }
      pclose(getroot);
    }
  }
  pclose(checkroot);

  // * 4 for quadruple voltage values
  int igfxvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt)) * 4;
  int igpupower = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower));
  int igfxclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock));
  int imemmvddvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt)) * 4;
  int imemvddcivolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt)) * 4;
  int imemclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock));
  int isocvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt)) * 4;
  int isocclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock));

  char writecmd[512];
  g_snprintf(writecmd, sizeof(writecmd), "%s %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d", uppwrite,\
  gfxvoltset, igfxvolt,\
  gpupowerset, igpupower,\
  gfxclockset, igfxclock,\
  memmvddvoltset, imemmvddvolt,\
  memvddcivoltset, imemvddcivolt,\
  memclockset, imemclock,\
  socvoltset, isocvolt,\
  socclockset, isocclock);
  printf("%s\n", writecmd);

  FILE *setvalues = popen(writecmd, "r");
  if (setvalues != NULL) {
  while(fgets(response, sizeof response, setvalues)){
    printf("%s\n", response);
  }
  pclose(setvalues);
  gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Settings successfully applied", -1);
  gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
  }
}