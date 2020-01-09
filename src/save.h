void on_btn_perm_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  // TODO: check if no access to systemd -> dialog make your own startup files

  int igfxvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt)) * 4;
  int igpupower = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower));
  int igfxclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock));
  int imemmvddvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt)) * 4;
  int imemvddcivolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt)) * 4;
  int imemclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock));
  int isocvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt)) * 4;
  int isocclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock));

  char scriptpath[512];
  char chmodbashscript[PATH_MAX];
  
      char writecmd[512];
      // fill data to bashscript with upp commands
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

      snprintf(scriptpath, sizeof(scriptpath), "%s/startup_script_card%d.sh", dirname, card_num);
      snprintf(chmodbashscript, sizeof(chmodbashscript), "chmod +x %s", scriptpath);

      // write bashscript
      FILE *fbashscript = fopen(scriptpath, "w+");
      fprintf(fbashscript, "chmod 646 /sys/class/drm/card%d/device/pp_table\nsudo -u %s %s", card_num, p->pw_name, writecmd);
      if (fbashscript != NULL) {
        printf("Bash script %s created successfully\n", scriptpath);
        // make executable
        FILE *fchmodbashscript = popen(chmodbashscript, "r");
        if (fchmodbashscript != NULL) {
          printf("Bash script %s set to executable\n", scriptpath);
          char response[2048];
          // TODO: check if systemd file is properly loaded, file can exist but service is not running  
          // check if systemd file exists, no use overwriting
          char servicepath[512];
          snprintf(servicepath, sizeof(servicepath), "/etc/systemd/system/powerupp%d.service", card_num);

          if(access(servicepath, F_OK) == 0) {
              printf("Systemd service file exists, not touching\n");
              gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Bash script successfully updated", -1);
              gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);          
          }
          else {
            //write systemd file
            char systemdcmd[PATH_MAX];
            snprintf(systemdcmd, PATH_MAX, "pkexec bash -c \"echo '[Unit]\n\
Description=PowerUPP\n\n\
[Service]\n\
Type=simple\n\
ExecStart=/bin/bash %s\n\n\
[Install]\n\
WantedBy=multi-user.target' > /etc/systemd/system/powerupp%d.service\
;chmod 644 /etc/systemd/system/powerupp%d.service\
;systemctl start powerupp%d.service\
;systemctl enable powerupp%d.service\"", scriptpath, card_num, card_num, card_num, card_num);
            FILE *writesystemd = popen (systemdcmd, "r"); 
            if (writesystemd != NULL) {
              while(fgets(response, sizeof response, writesystemd)){
                  printf("%s\n", response);  
              }
            gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Successfully created systemd file", -1);
            gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);  
            }
            else {
              printf("Unable to create systemd service file\n");
              gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Unable to create systemd service file", -1);
              gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE); 
            }
          pclose(writesystemd);
        } 
        }
        else {
          printf("Unable to set bash script to executable\n");
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Unable to set bash script to executable", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE); 
        }
      pclose(fchmodbashscript);
      }
      else {
        printf("Unable to create bash script\n");
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Unable to create bash script", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
      }
      fclose(fbashscript);
}