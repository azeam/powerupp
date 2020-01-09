void on_btn_defaults_clicked(GtkButton *button, app_widgets *app_wdgts)
{
        if(access(valuespath, F_OK) != -1 && access(limitspath, F_OK) != -1) {
            // default values file exists
            printf("Default settings files %s and %s exist, loading data\n", valuespath, limitspath);

            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gfxvolt), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gpupower), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gfxclock), FALSE);


            char countlines;
            int lines = 0;

            FILE *defaultlimits = fopen(limitspath, "r");
            
            // check if default limits = number of settings to fill
            while(!feof(defaultlimits)){
              countlines = fgetc(defaultlimits);
              if(countlines == '\n') {
              lines++;
              }
            }
            fclose(defaultlimits);

            defaultlimits = fopen(limitspath, "r");
            if(lines == numberoflimits) {
            
              char bval[1024];
              int cur_line = 0;
           
              while(fgets(bval, sizeof bval, defaultlimits)) {
                if (cur_line == gfxvoltlimitlowerorder - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower Gfx voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvolt), gval);
                  gfxvoltlimitlower = ival;
                  }
                else if (cur_line == gfxvoltlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper Gfx voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvolt), gval);
                  gfxvoltlimitupper = ival;
                }
                else if (cur_line == gpupowerlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower GPU power limit: %s W\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gpupower), gval);
                  gpupowerlimitupper = ival;
                }
                  else if (cur_line == gpupowerlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper GPU power limit: %s W\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gpupower), gval);
                  gpupowerlimitupper = ival;
                }
                else if (cur_line == gfxclocklimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower Gfx clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxclock), gval);
                  gfxclocklimitlower = ival;
                }
                else if (cur_line == gfxclocklimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper Gfx clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxclock), gval);
                  gfxclocklimitupper = ival;
                }  
                else if (cur_line == memmvddvoltlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
                  memmvddvoltlimitlower = ival;
                } 
                else if (cur_line == memmvddvoltlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
                  memmvddvoltlimitupper = ival;
                }
                 else if (cur_line == memvddcivoltlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
                  memvddcivoltlimitlower = ival;
                } 
                else if (cur_line == memvddcivoltlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
                  memvddcivoltlimitupper = ival;
                }
                else if (cur_line == memclocklimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock), gval);
                  memclocklimitlower = ival;
                }
                else if (cur_line == memclocklimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock), gval);
                  memclocklimitupper = ival;
                }
                else if (cur_line == socvoltlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower SoC voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), gval);
                  socvoltlimitlower = ival;
                } 
                else if (cur_line == socvoltlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper SoC voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), gval);
                  socvoltlimitupper = ival;
                }
                 else if (cur_line == socclocklimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower SoC clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), gval);
                  socclocklimitlower = ival;
                }
                else if (cur_line == socclocklimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper SoC clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), gval);
                  socclocklimitupper = ival;
                }        
            cur_line++;
            }
             fclose(defaultlimits);
            }
            else {
              gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Invalid default limits file. Reload your active settings", -1);
              gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
              printf("Default limits file is incomplete or broken, not loading. Try to reload your active settings\n");
            }     
             
            // reset line count 
            lines = 0;

            FILE *defaultvalues = fopen(valuespath, "r");
          
            // check if default values = number of settings to fill
            while(!feof(defaultvalues)){
              countlines = fgetc(defaultvalues);
              if(countlines == '\n') {
              lines++;
              }
            }
            fclose(defaultvalues);
            

            defaultvalues = fopen(valuespath, "r");
            if(lines == numberofvalues) {
            
              char bval[1024];
              int cur_line = 0;

              while(fgets(bval, sizeof bval, defaultvalues)) {
                if (cur_line == gfxvoltorder - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max Gfx voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxvolt),TRUE);
                  
                  if (gfxvoltlimitlower > ival || ival > gfxvoltlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gfxvolt), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt), gval);
                  }
                else if (cur_line == gpupowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default max GPU power: %s W\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gpupower),TRUE);
                  if (gpupowerlimitlower > ival || ival > gpupowerlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gpupower), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower), gval);
                }
                else if (cur_line == gfxclockorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default max Gfx clock: %s MHz\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxclock),TRUE);
                  
                  if (gfxclocklimitlower > ival || ival > gfxclocklimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gfxclock), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock), gval);
                }
                else if (cur_line == memmvddvoltorder - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem MVDD voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt),TRUE);
                  
                  if (memmvddvoltlimitlower > ival || ival > memmvddvoltlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memmvddvolt), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt), gval);
                }
                else if (cur_line == memvddcivoltorder - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem VDDCI voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt),TRUE);
                  
                  if (memvddcivoltlimitlower > ival || ival > memvddcivoltlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memvddcivolt), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt), gval);
                }
                else if (cur_line == memclockorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default max mem clock: %s MHz\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock),TRUE);
                  
                  if (memclocklimitlower > ival || ival > memclocklimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memclock), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock), gval);
                }
                else if (cur_line == socvoltorder - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max SoC voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socvolt),TRUE);
                  
                  if (socvoltlimitlower > ival || ival > socvoltlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_socvolt), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt), gval);
                }
                else if (cur_line == socclockorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default max SoC clock: %s MHz\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socclock),TRUE);
                  
                  if (socclocklimitlower > ival || ival > socclocklimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_socclock), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock), gval);
                }
            cur_line++;
            }
          gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(g_btn_perm), TRUE); 
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Default values loaded", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
        }
        else {
          printf("Default values file is incomplete or broken, not loading. Try to reload your active settings\n");
          gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Invalid default values file. Reload your active settings", -1);
          gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), FALSE);
          gtk_widget_set_sensitive(GTK_WIDGET(g_btn_perm), FALSE);
        }     
        fclose(defaultvalues);
        }
      else {
        // file doesn't exist
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "No default settings files. Reload your active settings", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
        printf("Default settings files don't exist, not loading. Try to reload your active settings\n");
        gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(g_btn_perm), FALSE);
      }   
}