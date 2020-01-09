void on_btn_active_clicked(GtkButton *button, app_widgets *app_wdgts)
{
int error = 0;
char bdata[1024];

// untoggle to set them as they should be at the end, rare but a bit odd situations could happen otherwise
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gfxvolt), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gfxclock), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gpupower), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memmvddvolt), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memvddcivolt), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memclock), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_socvolt), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_socclock), FALSE);

// read and set limits, have to be set before the values
FILE *fgfxvoltlimits = popen(gfxvoltlimits, "r");
  if(fgfxvoltlimits != NULL) {
  int cur_line = 0;

  while(fgets(bdata, sizeof bdata, fgfxvoltlimits)){
    if(cur_line == 0) {
      sscanf (bdata, "%s\n",bdata);
      int ival = atoi(bdata);
      gdouble gval = ival;
      if(ival > 0) {
        printf("Max Gfx volt upper limit: %g mV\n", gval);
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvolt), gval);
        gfxvoltlimitupper = ival;
      }
      else {
        printf("Max Gfx volt upper limit: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gfxvolt), "Error");
        error = 1;
        break;
      }
    }
    else if (cur_line == 1) {
      sscanf (bdata, "%s\n",bdata);
      int ival = atoi(bdata);
      gdouble gval = ival;
      if(ival > 0) {
        printf("Max Gfx volt lower limit: %g mV\n", gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvolt), gval);
        gfxvoltlimitlower = ival;
      }
      else {
        printf("Max Gfx volt lower limit: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gfxvolt), "Error");
        error = 1;
        break;
      }
    }
    cur_line++;  
  }
pclose(fgfxvoltlimits);
}

  // prefetch the power value to calculate the percentage limit
  // power limits WILL be wrong if power value has been set elsewhere before running for the first time, but I see no other way to 
  // get around this except hard-coding the stock value for different models. Seems to be a fairly safe setting to go above the limits though.
  // also fetch values for values without limits, to use values as limits, check if defaults are saved first
  float powerval;
  char bpower[1024];
  char bmemmvddvolt[1024];
  char bmemvddcivolt[1024];
  char bsocvolt[1024];
  char bsocclock[1024];
  char bdefaults[1024];

  // check if default settings exist, load value from there                         
  if(access(valuespath, F_OK) != -1) {

    // Default settings files exist
    char countlines;
    int lines = 0;

    // confirm that the files have the same number of items as they should
    FILE *defaultvalues = fopen(valuespath, "r");
    while(!feof(defaultvalues)){
      countlines = fgetc(defaultvalues);
      if(countlines == '\n') {
        lines++;
      }
    }

    fclose(defaultvalues);
    int cur_def_line = 0;

    if (lines == numberofvalues){
    FILE *defaultvalues = fopen(valuespath, "r");
    while(fgets(bdefaults, sizeof bdefaults, defaultvalues)){
      if(cur_def_line == gpupowerorder - 1) {
        powerval = atoi(bdefaults);
        printf("Default power value exists, calculating power limits based on that\n");
      }
      else if(cur_def_line == memmvddvoltorder - 1) {
        memmvddvoltlimitlower = atoi(bdefaults);
        memmvddvoltlimitupper = atoi(bdefaults);
        printf("testvolt %d\n", memmvddvoltlimitlower);
        gdouble gval = memmvddvoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
        printf("Default mem MVDD voltage value exists, calculating mem MVDD voltage limits based on that\n");
        printf("Max mem MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem MVDD voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == memvddcivoltorder - 1) {
        memvddcivoltlimitlower = atoi(bdefaults);
        memvddcivoltlimitupper = atoi(bdefaults);
        gdouble gval = memvddcivoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
        printf("Default mem VDDCI voltage value exists, calculating mem VDDCI voltage limits based on that\n");
        printf("Max mem VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem VDDCI voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == socvoltorder - 1) {
        socvoltlimitlower = atoi(bdefaults);
        socvoltlimitupper = atoi(bdefaults);
        gdouble gval = socvoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), gval);
        printf("Default SoC voltage value exists, calculating SoC voltage limits based on that\n");
        printf("Max SoC voltage lower limit: %g mV\n", gval);
        printf("Max SoC voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == socclockorder - 1) {
        socclocklimitlower = atoi(bdefaults);
        socclocklimitupper = atoi(bdefaults);
        gdouble gval = socclocklimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), gval);
        printf("Default SoC clock value exists, calculating SoC clock limits based on that\n");
        printf("Max SoC clock lower limit: %g MHz\n", gval);
        printf("Max SoC clock upper limit: %g MHz\n", gval);
      }
      cur_def_line++;
    }
    fclose(defaultvalues);
    }
    else {
      // corrupt default settings, get the data from pp table. This is probably the most insecure scenario for setting safe limits but should rarely happen. Neither option is good though.
      FILE *fgpupower = popen(gpupower, "r");
      if(fgpupower) {
        if(fgets(bdefaults, sizeof bdefaults, fgpupower)){
          powerval = atoi(bdefaults);
          printf("Default but invalid power value exists, using value from pp table\n");
        }
        pclose(fgpupower);
      }
      
      FILE *fmemmvddvolt = popen(memmvddvolt, "r");
      if(fmemmvddvolt) {
        if(fgets(bmemmvddvolt, sizeof bmemmvddvolt, fmemmvddvolt)){
        memmvddvoltlimitlower = atoi(bmemmvddvolt) / 4;
        memmvddvoltlimitupper = atoi(bmemmvddvolt) / 4;
        gdouble gval = memmvddvoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
        printf("Default but invalid mem MVDD voltage value exists, using value from pp table\n");
        printf("Max mem MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem MVDD voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemmvddvolt);
      }

      FILE *fmemvddcivolt = popen(memvddcivolt, "r");
      if(fmemvddcivolt) {
        if(fgets(bmemvddcivolt, sizeof bmemvddcivolt, fmemvddcivolt)){
        memvddcivoltlimitlower = atoi(bmemvddcivolt) / 4;
        memvddcivoltlimitupper = atoi(bmemvddcivolt) / 4;
        gdouble gval = memvddcivoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
        printf("Default but invalid mem VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem VDDCI voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemvddcivolt);
      }

      FILE *fsocvolt = popen(socvolt, "r");
      if(fsocvolt) {
        if(fgets(bsocvolt, sizeof bsocvolt, fsocvolt)){
        socvoltlimitlower = atoi(bsocvolt) / 4;
        socvoltlimitupper = atoi(bsocvolt) / 4;
        gdouble gval = socvoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), gval);
        printf("Default but invalid SoC voltage value exists, using value from pp table\n");
        printf("Max SoC voltage lower limit: %g mV\n", gval);
        printf("Max SoC voltage upper limit: %g mV\n", gval);
        }
        pclose(fsocvolt);
      }

      FILE *fsocclock = popen(socclock, "r");
      if(fsocclock) {
        if(fgets(bsocclock, sizeof bsocclock, fsocclock)){
        socclocklimitlower = atoi(bsocclock);
        socclocklimitupper = atoi(bsocclock);
        gdouble gval = socclocklimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), gval);
        printf("Default but invalid SoC clock value exists, using value from pp table\n");
        printf("Max SoC clock lower limit: %g MHz\n", gval);
        printf("Max SoC clock upper limit: %g MHz\n", gval);
        }
        pclose(fsocclock);
      }

    }
  }
  else {
  // no default settings, get the data from pp table  
    FILE *fgpupower = popen(gpupower, "r");
    if(fgpupower) {
      if(fgets(bpower, sizeof bpower, fgpupower)){
        powerval = atoi(bpower);
        printf("No default power value exists, using value from pp table\n");
      }
      pclose(fgpupower);
    }

    FILE *fmemmvddvolt = popen(memmvddvolt, "r");
    if(fmemmvddvolt) {
      if(fgets(bmemmvddvolt, sizeof bmemmvddvolt, fmemmvddvolt)){
        memmvddvoltlimitlower = atoi(bmemmvddvolt) / 4;
        memmvddvoltlimitupper = atoi(bmemmvddvolt) / 4;
        gdouble gval = memmvddvoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
        printf("No default mem MVDD voltage value exists, using value from pp table\n");
        printf("Max mem MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem MVDD voltage upper limit: %g mV\n", gval);
      }
      pclose(fmemmvddvolt);
    }

    FILE *fmemvddcivolt = popen(memvddcivolt, "r");
    if(fmemvddcivolt) {
      if(fgets(bmemvddcivolt, sizeof bmemvddcivolt, fmemvddcivolt)){
        memvddcivoltlimitlower = atoi(bmemvddcivolt) / 4;
        memvddcivoltlimitupper = atoi(bmemvddcivolt) / 4;
        gdouble gval = memvddcivoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
        printf("No default mem VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem VDDCI voltage upper limit: %g mV\n", gval);
      }
      pclose(fmemvddcivolt);
    }

    FILE *fsocvolt = popen(socvolt, "r");
    if(fsocvolt) {
      if(fgets(bsocvolt, sizeof bsocvolt, fsocvolt)){
        socvoltlimitlower = atoi(bsocvolt) / 4;
        socvoltlimitupper = atoi(bsocvolt) / 4;
        gdouble gval = socvoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), gval);
        printf("No default SoC voltage value exists, using value from pp table\n");
        printf("Max SoC voltage lower limit: %g mV\n", gval);
        printf("Max SoC voltage upper limit: %g mV\n", gval);
      }
      pclose(fsocvolt);
    }

    FILE *fsocclock = popen(socclock, "r");
    if(fsocclock) {
      if(fgets(bsocclock, sizeof bsocclock, fsocclock)){
        socclocklimitlower = atoi(bsocclock);
        socclocklimitupper = atoi(bsocclock);
        gdouble gval = socclocklimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), gval);
        printf("No default SoC clock value exists, using value from pp table\n");
        printf("Max SoC clock lower limit: %g MHz\n", gval);
        printf("Max SoC clock upper limit: %g MHz\n", gval);
      }
      pclose(fsocclock);
    }

  }
    

// set limits to adjustments
FILE *fgpupowerlimits = popen(gpupowerlimits, "r");
  if(fgpupowerlimits != NULL) {
  int cur_line = 0;

  while(fgets(bdata, sizeof bdata, fgpupowerlimits)){  
    if(cur_line == 0) {
      sscanf (bdata, "%s\n",bdata);
      // +x percent of value
      float ival = powerval + (powerval * (float)atoi(bdata) / (float)100);
      gdouble gval = ival;
      if(ival > 0) {
        printf("Max GPU power upper limit: %g W\n", gval);
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gpupower), gval);
        gpupowerlimitupper = ival;
      }
      else {
        printf("Max GPU power upper limit: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gpupower), "Error");
        error = 1;
        break;
      }
    }
    else if (cur_line == 1) {
    sscanf (bdata, "%s\n",bdata);
    // -x percent of value
    float ival = powerval - (powerval * (1 - (float)atoi(bdata) / (float)100));
    gdouble gval = ival;
      if(ival > 0) {
        printf("Max GPU power lower limit: %g W\n", gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gpupower), gval);
        gpupowerlimitlower = ival;
      }
      else {
        printf("Max GPU power lower limit: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gpupower), "Error");
        error = 1;
        break;
      }
    }
    cur_line++;  
  }
pclose(fgpupowerlimits);
}

FILE *fgfxclocklimits = popen(gfxclocklimits, "r");
  if(fgfxclocklimits) {
  int cur_line = 0;

  while(fgets(bdata, sizeof bdata, fgfxclocklimits)){
    if(cur_line == 0) {
      sscanf (bdata, "%s\n",bdata);
      int ival = atoi(bdata);
      gdouble gval = ival;
      if(ival > 0) {
        printf("Max Gfx clock upper limit: %g MHz\n", gval);
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxclock), gval);
        gfxclocklimitupper = ival;
      }
      else {
        printf("Max Gfx clock upper limit: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gfxclock), "Error");
        error = 1;
        break;
      }
    }
    else if (cur_line == 1) {
      sscanf (bdata, "%s\n",bdata);
      int ival = atoi(bdata);
      gdouble gval = ival;
      if(ival > 0) {
        printf("Max Gfx clock lower limit: %g MHz\n", gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxclock), gval);
        gfxclocklimitlower = ival;
      }
      else {
        printf("Max Gfx clock lower limit: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gfxclock), "Error");
        error = 1;
        break;
      }
    }
    cur_line++;  
  }
pclose(fgfxclocklimits);
}

FILE *fmemclocklimits = popen(memclocklimits, "r");
  if(fmemclocklimits) {
  int cur_line = 0;

  while(fgets(bdata, sizeof bdata, fmemclocklimits)){
    if(cur_line == 0) {
      sscanf (bdata, "%s\n",bdata);
      int ival = atoi(bdata);
      gdouble gval = ival;
      if(ival > 0) {
        printf("Max mem clock upper limit: %g MHz\n", gval);
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock), gval);
        memclocklimitupper = ival;
      }
      else {
        printf("Max mem clock upper limit: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memclock), "Error");
        error = 1;
        break;
      }
    }
    else if (cur_line == 1) {
      sscanf (bdata, "%s\n",bdata);
      int ival = atoi(bdata);
      gdouble gval = ival;
      if(ival > 0) {
        printf("Max mem clock lower limit: %g MHz\n", gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock), gval);
        memclocklimitlower = ival;
      }
      else {
        printf("Max mem clock lower limit: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memclock), "Error");
        error = 1;
        break;
      }
    }
    cur_line++;  
  }
pclose(fmemclocklimits);
}
  



if (error == 0) {
// read values and update spin buttons
FILE *fgfxvolt = popen(gfxvolt, "r");
  if(fgfxvolt) {
  while(fgets(bdata, sizeof bdata, fgfxvolt)){
    // AMD uses quadruple values for the voltages, this is by design according to sibradzic. Display actual values.
    int quarterval = atoi(bdata) / 4;
    gdouble val = quarterval;
    if(quarterval > 0) {
      printf("Max Gfx voltage: %g mV\n", val);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxvolt),TRUE); 
      if (gfxvoltlimitlower > quarterval || quarterval > gfxvoltlimitupper) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gfxvolt), TRUE);
      }
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt), val);
    }
    else {
      printf("Max Gfx voltage: Error, 0 or no value\n");
      gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gfxvolt), "Error");
      error = 1;
      break;
    }     
  }
  pclose(fgfxvolt);
  }

  FILE *fgpupower = popen(gpupower, "r");
  if(fgpupower) {

  while(fgets(bdata, sizeof bdata, fgpupower)){
    int ival = atoi(bdata);
    gdouble gval = ival;
    if(ival > 0) {
      printf("Max GPU power: %d W\n", ival);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gpupower),TRUE);
      if (gpupowerlimitlower > ival || ival > gpupowerlimitupper) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gpupower), TRUE);
      } 
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower), gval); 
    }
    else {
      printf("Max GPU power: Error, 0 or no value\n");
      gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gpupower), "Error");
      error = 1;
      break;
    }     
  }
  pclose(fgpupower);
  }

  FILE *fgfxclock = popen(gfxclock, "r");
  if(fgfxclock) {

  while(fgets(bdata, sizeof bdata, fgfxclock)){
    int ival = atoi(bdata);
    gdouble gval = ival;
    if(ival > 0) {
      printf("Max Gfx clock: %d MHz\n", ival);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxclock),TRUE);
      if (gfxclocklimitlower > ival || ival > gfxclocklimitupper) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_gfxclock), TRUE);
      } 
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock), gval); 
    }
    else {
      printf("Max Gfx clock: Error, 0 or no value\n");
      gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gfxclock), "Error");
      error = 1;
      break;
    }     
  }
  pclose(fgfxclock);
  }


  FILE *fmemmvddvolt = popen(memmvddvolt, "r");
    if(fmemmvddvolt) {
    while(fgets(bdata, sizeof bdata, fmemmvddvolt)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max mem MVDD voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt),TRUE); 
        if (memmvddvoltlimitlower > quarterval || quarterval > memmvddvoltlimitupper) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memmvddvolt), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt), val);
      }
      else {
        printf("Max mem MVDD voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memmvddvolt), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fmemmvddvolt);
  }

  FILE *fmemvddcivolt = popen(memvddcivolt, "r");
    if(fmemvddcivolt) {
    while(fgets(bdata, sizeof bdata, fmemvddcivolt)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max mem VDDCI voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt),TRUE); 
        if (memvddcivoltlimitlower > quarterval || quarterval > memvddcivoltlimitupper) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memvddcivolt), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt), val);
      }
      else {
        printf("Max mem VDDCI voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memvddcivolt), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fmemvddcivolt);
  }


  FILE *fmemclock = popen(memclock, "r");
  if(fmemclock) {

  while(fgets(bdata, sizeof bdata, fmemclock)){
    int ival = atoi(bdata);
    gdouble gval = ival;
    if(ival > 0) {
      printf("Max mem clock: %d MHz\n", ival);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock),TRUE);
      if (memclocklimitlower > ival || ival > memclocklimitupper) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_memclock), TRUE);
      } 
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock), gval); 
    }
    else {
      printf("Max mem clock: Error, 0 or no value\n");
      gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memclock), "Error");
      error = 1;
      break;
    }     
  }
  pclose(fmemclock);
  }


  FILE *fsocvolt = popen(socvolt, "r");
    if(fsocvolt) {
    while(fgets(bdata, sizeof bdata, fsocvolt)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max SoC voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socvolt),TRUE); 
        if (socvoltlimitlower > quarterval || quarterval > socvoltlimitupper) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_socvolt), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt), val);
      }
      else {
        printf("Max SoC voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_socvolt), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fsocvolt);
  }

   FILE *fsocclock = popen(socclock, "r");
    if(fsocclock) {
    while(fgets(bdata, sizeof bdata, fsocclock)){
      int quarterval = atoi(bdata);
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max SoC clock: %g MHz\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socclock),TRUE); 
        if (socclocklimitlower > quarterval || quarterval > socclocklimitupper) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_socclock), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock), val);
      }
      else {
        printf("Max SoC clock: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_socclock), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fsocclock);
  }
}
// if getting data was ok, save it as default unless a default settings file already exists
  int goodtowrite = 0;
  if (error == 0) {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Active values loaded", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
    
    if(access(valuespath, F_OK) != -1 && access(limitspath, F_OK) != -1) {
        // Default settings files exist

        // TODO: possibly check if default are < current, then append by echo >> file instead of overwriting
        // futureproofing, if adding more options (or if not good in other ways) the old settings files will be overwritten with current
        char countlines;
        int valueslines = 0;
        FILE *defaultvalues = fopen(valuespath, "r");
        while(!feof(defaultvalues)){
          countlines = fgetc(defaultvalues);
          if(countlines == '\n') {
          valueslines++;
          }
        }
        fclose(defaultvalues);

        int defaultlines = 0;
        FILE *defaultlimits = fopen(limitspath, "r");
        while(!feof(defaultlimits)){
          countlines = fgetc(defaultlimits);
          if(countlines == '\n') {
          defaultlines++;
          }
        }

        fclose(defaultlimits);

        if ((valueslines == numberofvalues) && (defaultlines == numberoflimits)){
          printf("Default settings files exist, no changes made\n");
        }
        else {
          goodtowrite = 1;
          printf("Default settings files exist but are outdated or invalid, saving current values as defaults\n");
        }
    } 
    else {
    struct stat st = {0};
    if (stat(dirname, &st) == -1) {
        // TODO: confirm that it works if .config folder doesn't exist, otherwise send mkdir -p
        if(mkdir(dirname, 0755) != -1){
          //Directory doesn't exist, successfully created
          printf("Default settings files don't exist, saving current values as defaults\n");
          goodtowrite = 1;         
        }
        else{
          printf("Not able to create directory %s\n", dirname);
        }
    }
    else {
      //Directory exists but files don't, save the files
      printf("Default settings files don't exist, saving current values as defaults\n");
      goodtowrite = 1;
    }
  }
  if (goodtowrite){
    // Fill data to values
    int igfxvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt));
    int igpupower = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower));
    int igfxclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock));
    int imemmvddvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt));
    int imemvddcivolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt));
    int imemclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock));
    int isocvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt));
    int isocclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock));
  
    // save to files
    printf("Saving current values in %s and limits in %s as defaults\n", valuespath, limitspath);

    FILE *defaultvalues = fopen(valuespath, "w+");
  
    // here the order is important !!!
    fprintf(defaultvalues, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", igfxvolt, igfxclock, igpupower, imemmvddvolt, imemvddcivolt, imemclock, isocvolt, isocclock);

    FILE *defaultlimits = fopen(limitspath, "w+");
    // here the order is important !!!
    fprintf(defaultlimits, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",\
    gfxvoltlimitlower, gfxvoltlimitupper,\
    gfxclocklimitlower, gfxclocklimitupper,\
    gpupowerlimitlower, gpupowerlimitupper,\
    memmvddvoltlimitlower, memmvddvoltlimitupper,\
    memvddcivoltlimitlower, memvddcivoltlimitupper,\
    memclocklimitlower, memclocklimitupper,\
    socvoltlimitlower, socvoltlimitupper,\
    socclocklimitlower, socclocklimitupper);
    
    if (defaultlimits && defaultvalues != NULL) {
      printf("Default data saved successfully\n");
      // if ok, enable load defaults button
      gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Active values loaded and saved as defaults", -1);
      gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
      gtk_widget_set_sensitive(GTK_WIDGET(g_btn_defaults), TRUE);
    }
    else {
      printf("Unable to save defaults settings\n");
      gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Unable to save default settings", -1);
      gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
    }
    fclose(defaultvalues);
    fclose(defaultlimits); 
  } 
    // if ok set apply and save buttons enabled
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_perm), TRUE); 
  }
  else {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error loading data, default settings not saved", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
    printf("Errors while loading the data, no default settings have been saved\n");
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_perm), FALSE);
  }  
}