void on_btn_defaults_clicked(GtkButton *button, app_widgets *app_wdgts)
{
        if(access(valuespath, F_OK) != -1 && access(limitspath, F_OK) != -1) {
            // default values file exists
            printf("Default settings files %s and %s exist, loading data\n", valuespath, limitspath);

            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), FALSE);
      
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
                  printf("Default lower max Gfx voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvolt), gval);
                  gfxvoltlimitlower = ival;
                  }
                else if (cur_line == gfxvoltlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper max Gfx voltage limit: %s mV\n", bval);
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
                  printf("Default lower mem DPM 3 MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
                  memmvddvoltlimitlower = ival;
                } 
                else if (cur_line == memmvddvoltlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 3 MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
                  memmvddvoltlimitupper = ival;
                }
                 else if (cur_line == memvddcivoltlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 3 VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
                  memvddcivoltlimitlower = ival;
                } 
                else if (cur_line == memvddcivoltlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 3 VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
                  memvddcivoltlimitupper = ival;
                }
                else if (cur_line == memclocklimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 3 clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock), gval);
                  memclocklimitlower = ival;
                }
                else if (cur_line == memclocklimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 3 clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock), gval);
                  memclocklimitupper = ival;
                }
                else if (cur_line == socvoltlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower max SoC voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), gval);
                  socvoltlimitlower = ival;
                } 
                else if (cur_line == socvoltlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper max SoC voltage limit: %s mV\n", bval);
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

                 else if (cur_line == voltoffsetlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower static voltage offset limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_voltoffset), gval);
                  voltoffsetlimitlower = ival;
                }
                else if (cur_line == voltoffsetlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper static voltage offset limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_voltoffset), gval);
                  voltoffsetlimitupper = ival;
                }  

                 else if (cur_line == gfxvoltminlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower min Gfx voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvoltmin), gval);
                  gfxvoltminlimitlower = ival;
                }
                else if (cur_line == gfxvoltminlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper min Gfx voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvoltmin), gval);
                  gfxvoltminlimitupper = ival;
                }  

                 else if (cur_line == socvoltminlimitlowerorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower min SoC voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvoltmin), gval);
                  socvoltminlimitlower = ival;
                }
                else if (cur_line == socvoltminlimitupperorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper min SoC voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvoltmin), gval);
                  socvoltminlimitupper = ival;
                }  
                // DPM 0
                else if (cur_line == memmvddvoltlimitlowerorder0 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 0 MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt0), gval);
                  memmvddvoltlimitlower0 = ival;
                } 
                else if (cur_line == memmvddvoltlimitupperorder0 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 0 MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt0), gval);
                  memmvddvoltlimitupper0 = ival;
                }
                 else if (cur_line == memvddcivoltlimitlowerorder0 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 0 VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt0), gval);
                  memvddcivoltlimitlower0 = ival;
                } 
                else if (cur_line == memvddcivoltlimitupperorder0 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 0 VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt0), gval);
                  memvddcivoltlimitupper0 = ival;
                }
                else if (cur_line == memclocklimitlowerorder0 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 0 clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock0), gval);
                  memclocklimitlower0 = ival;
                }
                else if (cur_line == memclocklimitupperorder0 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 0 clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock0), gval);
                  memclocklimitupper0 = ival;
                }
                //DPM 1
                else if (cur_line == memmvddvoltlimitlowerorder1 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 1 MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt1), gval);
                  memmvddvoltlimitlower1 = ival;
                } 
                else if (cur_line == memmvddvoltlimitupperorder1 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 1 MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt1), gval);
                  memmvddvoltlimitupper1 = ival;
                }
                 else if (cur_line == memvddcivoltlimitlowerorder1 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 1 VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt1), gval);
                  memvddcivoltlimitlower1 = ival;
                } 
                else if (cur_line == memvddcivoltlimitupperorder1 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 1 VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt1), gval);
                  memvddcivoltlimitupper1 = ival;
                }
                else if (cur_line == memclocklimitlowerorder1 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 1 clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock1), gval);
                  memclocklimitlower1 = ival;
                }
                else if (cur_line == memclocklimitupperorder1 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 1 clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock1), gval);
                  memclocklimitupper1 = ival;
                }
                //DPM 2
                else if (cur_line == memmvddvoltlimitlowerorder2 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 2 MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt2), gval);
                  memmvddvoltlimitlower2 = ival;
                } 
                else if (cur_line == memmvddvoltlimitupperorder2 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 2 MVDD voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt2), gval);
                  memmvddvoltlimitupper2 = ival;
                }
                 else if (cur_line == memvddcivoltlimitlowerorder2 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 2 VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt2), gval);
                  memvddcivoltlimitlower2 = ival;
                } 
                else if (cur_line == memvddcivoltlimitupperorder2 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 2 VDDCI voltage limit: %s mV\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt2), gval);
                  memvddcivoltlimitupper2 = ival;
                }
                else if (cur_line == memclocklimitlowerorder2 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default lower mem DPM 2 clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock2), gval);
                  memclocklimitlower2 = ival;
                }
                else if (cur_line == memclocklimitupperorder2 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default upper mem DPM 2 clock limit: %s MHz\n", bval);
                  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock2), gval);
                  memclocklimitupper2 = ival;
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
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock), gval);
                }
                else if (cur_line == memmvddvoltorder - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem DPM 3 MVDD voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt),TRUE);
                  if (memmvddvoltlimitlower > ival || ival > memmvddvoltlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt), gval);
                }
                else if (cur_line == memvddcivoltorder - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem DPM 3 VDDCI voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt),TRUE);
                  if (memvddcivoltlimitlower > ival || ival > memvddcivoltlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt), gval);
                }
                else if (cur_line == memclockorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default max mem DPM 3 clock: %s MHz\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock),TRUE);
                  if (memclocklimitlower > ival || ival > memclocklimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock), gval);
                }

                else if (cur_line == voltoffsetorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default static voltage offset: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_voltoffset),TRUE);    
                  if (voltoffsetlimitlower > ival || ival > voltoffsetlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_voltoffset), gval);
                }

                else if (cur_line == gfxvoltminorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default min Gfx voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxvoltmin),TRUE);    
                  if (gfxvoltminlimitlower > ival || ival > gfxvoltminlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvoltmin), gval);
                }

                else if (cur_line == socvoltminorder - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default min SoC voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socvoltmin),TRUE);    
                  if (socvoltminlimitlower > ival || ival > socvoltminlimitupper) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvoltmin), gval);
                }

                //DPM 0 
                else if (cur_line == memmvddvoltorder0 - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem DPM 0 MVDD voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt0),TRUE);
                  if (memmvddvoltlimitlower0 > ival || ival > memmvddvoltlimitupper0) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt0), gval);
                }
                else if (cur_line == memvddcivoltorder0 - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem DPM 0 VDDCI voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt0),TRUE);
                  if (memvddcivoltlimitlower0 > ival || ival > memvddcivoltlimitupper0) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt0), gval);
                }
                else if (cur_line == memclockorder0 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default max mem DPM 0 clock: %s MHz\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock0),TRUE);
                  if (memclocklimitlower0 > ival || ival > memclocklimitupper0) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock0), gval);
                }

                // DPM 1 
                else if (cur_line == memmvddvoltorder1 - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem DPM 1 MVDD voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt1),TRUE);
                  if (memmvddvoltlimitlower1 > ival || ival > memmvddvoltlimitupper1) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt1), gval);
                }
                else if (cur_line == memvddcivoltorder1 - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem DPM 1 VDDCI voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt1),TRUE);
                  if (memvddcivoltlimitlower1 > ival || ival > memvddcivoltlimitupper1) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt1), gval);
                }
                else if (cur_line == memclockorder1 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default max mem DPM 1 clock: %s MHz\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock1),TRUE);
                  if (memclocklimitlower1 > ival || ival > memclocklimitupper1) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock1), gval);
                }

                //DPM 2 
                else if (cur_line == memmvddvoltorder2 - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem DPM 2 MVDD voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt2),TRUE);
                  if (memmvddvoltlimitlower2 > ival || ival > memmvddvoltlimitupper2) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt2), gval);
                }
                else if (cur_line == memvddcivoltorder2 - 1) {             
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;
                  printf("Default max mem DPM 2 VDDCI voltage: %s mV\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt2),TRUE);
                  if (memvddcivoltlimitlower2 > ival || ival > memvddcivoltlimitupper2) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt2), gval);
                }
                else if (cur_line == memclockorder2 - 1) {
                  sscanf (bval, "%s\n",bval);
                  int ival = atoi(bval);
                  gdouble gval = ival;                
                  printf("Default max mem DPM 2 clock: %s MHz\n", bval);
                  gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock2),TRUE);
                  if (memclocklimitlower2 > ival || ival > memclocklimitupper2) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
                  }
                  gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock2), gval);
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