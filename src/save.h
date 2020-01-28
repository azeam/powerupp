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

  char powerhwmonwrite[248]; 
  char scriptpath[248];
  char chmodbashscript[512];
  char chmod2bashscript[512];
  char writecmd[4096];
  char bashscript[8192];
  char shebang[128] = "#!/bin/bash";
  char servicename[128];
  char systemdfile[2048];
  char systemdstart[248];
  char systemdenable[248];
  char systemdchmod[248];
  char systemdreload[248] = "systemctl daemon-reload";
  char systemdcmd[16384];
  char response[4096];
  char prefix[128] = "/usr/bin/";

  // set up cmd to write to hwmon because power pp table not working. Setting the pp table limit has to be done first though or it will not work, send hwmon cmd after
  snprintf(powerhwmonwrite, sizeof(powerhwmonwrite), "echo %d | tee /sys/class/hwmon/$(ls -1 /sys/class/drm/card%d/device/hwmon)/power1_cap", igpupower * 1000000, card_num);

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
    
  
  
  snprintf(scriptpath, sizeof(scriptpath), "%spowerupp_startup_script_card%d.sh", prefix, card_num);
  snprintf(bashscript, sizeof(bashscript), "echo '%s\nchmod 646 /sys/class/drm/card%d/device/pp_table\nsudo -u %s %s\nchmod 644 /sys/class/drm/card%d/device/pp_table\n%s' > %s", shebang, card_num, p->pw_name, writecmd, card_num, powerhwmonwrite, scriptpath);
  snprintf(chmodbashscript, sizeof(chmodbashscript), "chmod +x %s", scriptpath);
  snprintf(chmod2bashscript, sizeof(chmod2bashscript), "chmod 644 %s", scriptpath);
  snprintf(systemdchmod, sizeof(systemdchmod), "chmod 644 /etc/systemd/system/powerupp%d.service", card_num);
  snprintf(systemdstart, sizeof(systemdstart), "systemctl start powerupp%d.service", card_num);
  snprintf(systemdenable, sizeof(systemdenable), "systemctl enable powerupp%d.service", card_num);
  snprintf(systemdfile, sizeof(systemdfile), "echo '[Unit]\n\
Description=PowerUPP\n\n\
[Service]\n\
Type=simple\n\
ExecStart=/bin/bash %s\n\n\
[Install]\n\
WantedBy=multi-user.target' > /etc/systemd/system/powerupp%d.service", scriptpath, card_num);

  snprintf(systemdcmd, sizeof systemdcmd, "pkexec bash -c \"%s;%s;%s;%s;%s;%s;%s;%s\" 2>&1", bashscript, chmodbashscript, chmod2bashscript, systemdfile, systemdchmod, systemdenable, systemdstart, systemdreload);
  snprintf(servicename, sizeof servicename, "systemctl status powerupp%d.service", card_num);

  struct stat st = {0};

  //confirm systemd is present
  if (stat("/etc/systemd/system", &st) == -1) {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "No systemd found on this system, unable to save", -1), scriptpath;
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
  }
  else {
  FILE *writesystemd = popen (systemdcmd, "r"); 
  if (writesystemd != NULL) {
    int status = pclose(writesystemd);
    if (WEXITSTATUS(status) == 0) {
      FILE *systemdstatus = popen(servicename, "r");
      if (systemdstatus) {
        while(fgets(response, sizeof response, systemdstatus)){
          //check if systemd service status is ok
          if(strstr(response,"Active: active") != NULL || strstr(response,"Active: inactive") != NULL) {
            printf("Systemd service file successfully started\n");
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Systemd service successfully started", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
            break; 
          }
          else if(strstr(response,"Active: failed") != NULL) {
            printf("Systemd service file failed to start\n");
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Failed to start systemd service", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
            break;
          }
        }
      }
      pclose(systemdstatus);
    }
    else if (WEXITSTATUS(status) == 126) {
      gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Dialog cancelled, nothing saved", -1);
      gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
    }
  }
  else {
    printf("Unable to create systemd service file\n");
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Unable to create systemd service file", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE); 
  }
  }
}


