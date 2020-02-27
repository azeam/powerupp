void on_btn_perm_clicked(GtkButton *button, app_widgets *app_wdgts) {
  
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

  char powersysfswrite[248]; 
  char scriptpath[248];
  char chmodbashscript[512];
  char writecmd[4096];
  char bashscript[8192];
  char shebang[128] = "#!/bin/bash";
  char udevcmd[16384];
  char prefix[128] = "/usr/bin/";
  char cardid[32];
  char cardidpath[128];
  char udevrules[512];
  char udevpath[248];

  // set up cmd to write to sysfs because power pp table not working. Setting the pp table limit has to be done first though or it will not work, send sysfs cmd after
  snprintf(powersysfswrite, sizeof(powersysfswrite), "echo %d | tee /sys/class/hwmon/$(ls -1 /sys/class/drm/card%d/device/hwmon)/power1_cap", igpupower * 1000000, card_num);

  // fill data to bashscript with upp commands
  // full path to upp needed
  snprintf(writecmd, sizeof(writecmd), "%s%s %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%f %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d %s%d", prefix, uppwrite,\
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
  
  snprintf(udevpath, sizeof(udevpath), "/etc/udev/rules.d/80-powerupp%d.rules", card_num);
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

  snprintf(udevrules, sizeof(udevrules), "echo 'KERNEL==\\\"card%d\\\", SUBSYSTEM==\\\"drm\\\", DRIVERS==\\\"amdgpu\\\", ATTRS{device}==\\\"%s\\\", RUN+=\\\"/bin/bash /usr/bin/powerupp_startup_script_card%d.sh\\\"' > %s", card_num, cardid, card_num, udevpath);
  snprintf(scriptpath, sizeof(scriptpath), "%spowerupp_startup_script_card%d.sh", prefix, card_num);
  snprintf(bashscript, sizeof(bashscript), "echo '%s\nchmod 646 /sys/class/drm/card%d/device/pp_table\nsudo -u %s %s\nchmod 644 /sys/class/drm/card%d/device/pp_table\n%s' > %s", shebang, card_num, p->pw_name, writecmd, card_num, powersysfswrite, scriptpath);
  snprintf(chmodbashscript, sizeof(chmodbashscript), "chmod 755 %s", scriptpath);
  snprintf(udevcmd, sizeof(udevcmd), "pkexec bash -c \"%s;%s;%s\"", bashscript, chmodbashscript, udevrules);

  struct stat st = {0};

  //confirm udev default path is present
  if (stat("/etc/udev/rules.d/", &st) == -1) {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Path to udev not found, unable to save", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
  }
  else {
  FILE *writesystemd = popen (udevcmd, "r"); 
  if (writesystemd != NULL) {
    int status = pclose(writesystemd);
    if (WEXITSTATUS(status) == 0) {
      gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Settings successfully saved", -1);
      gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
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