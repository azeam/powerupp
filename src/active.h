void on_btn_active_clicked(GtkButton *button, app_widgets *app_wdgts) {
int error = 0;
char bdata[1024];

FILE *fuppdump = popen(uppdump, "r");
if (fuppdump == NULL) {
  error = 1;
}
pclose(fuppdump);

// untoggle to set it as it should be at the end
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), FALSE);

if (error == 0) {
  // read and set limits, have to be set before the values
  // prefetch the power value to calculate the percentage limit
  // power limits WILL be wrong if power value has been set elsewhere before running for the first time, but I see no other way to 
  // get around this except hard-coding the stock value for different models. Seems to be a fairly safe setting to go above the limits though.
  // also fetch values for values without limits, to use values as limits, check if defaults are saved first since it's better to load from there than pp table
  // if they have been changed
  float powerval;
  char bpower[1024];
  char bmemmvddvolt[1024];
  char bmemvddcivolt[1024];
  char bsocvolt[1024];
  char bsocclock[1024];
  char bsocvoltmin[1024];
  char bgfxvoltmin[1024];
  char bvoltoffset[1024];
  char bmemmvddvolt0[1024];
  char bmemvddcivolt0[1024];
  char bmemclock0[1024];
  char bmemmvddvolt1[1024];
  char bmemvddcivolt1[1024];
  char bmemclock1[1024];
  char bmemmvddvolt2[1024];
  char bmemvddcivolt2[1024];
  char bmemclock2[1024];

  char bdefaults[1024];

  int nodefault = 0;

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
        gdouble gval = memmvddvoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), gval);
        printf("Default mem DPM 3 MVDD voltage value exists, calculating mem MVDD voltage limits based on that\n");
        printf("Max mem DPM 3 MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 3 MVDD voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == memvddcivoltorder - 1) {
        memvddcivoltlimitlower = atoi(bdefaults);
        memvddcivoltlimitupper = atoi(bdefaults);
        gdouble gval = memvddcivoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), gval);
        printf("Default mem DPM 3 VDDCI voltage value exists, calculating mem VDDCI voltage limits based on that\n");
        printf("Max mem DPM 3 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 3 VDDCI voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == socvoltorder - 1) {
        socvoltlimitlower = atoi(bdefaults);
        socvoltlimitupper = atoi(bdefaults);
        gdouble gval = socvoltlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), gval);
        printf("Default SoC max voltage value exists, calculating SoC voltage limits based on that\n");
        printf("Max SoC voltage lower limit: %g mV\n", gval);
        printf("Max SoC voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == socvoltminorder - 1) {
        socvoltminlimitlower = atoi(bdefaults);
        socvoltminlimitupper = atoi(bdefaults);
        gdouble gval = socvoltminlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvoltmin), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvoltmin), gval);
        printf("Default SoC min voltage value exists, calculating SoC voltage limits based on that\n");
        printf("Min SoC voltage lower limit: %g mV\n", gval);
        printf("Min SoC voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == gfxvoltminorder - 1) {
        gfxvoltminlimitlower = atoi(bdefaults);
        gfxvoltminlimitupper = atoi(bdefaults);
        gdouble gval = gfxvoltminlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvoltmin), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvoltmin), gval);
        printf("Default Gfx min voltage value exists, calculating Gfx voltage limits based on that\n");
        printf("Min Gfx voltage lower limit: %g mV\n", gval);
        printf("Min Gfx voltage upper limit: %g mV\n", gval);
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
      else if(cur_def_line == voltoffsetorder - 1) {
        voltoffsetlimitlower = atof(bdefaults) * 1000;
        voltoffsetlimitupper = atof(bdefaults) * 1000;
        gdouble gval = voltoffsetlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_voltoffset), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_voltoffset), gval);
        printf("Default static voltage offset value exists, calculating voltage offset limits based on that\n");
        printf("Static voltage offset lower limit: %g mV\n", gval);
        printf("Static voltage offset upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == memmvddvoltorder0 - 1) {
        memmvddvoltlimitlower0 = atoi(bdefaults);
        memmvddvoltlimitupper0 = atoi(bdefaults);
        gdouble gval = memmvddvoltlimitupper0;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt0), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt0), gval);
        printf("Default mem DPM 0 MVDD voltage value exists, calculating mem MVDD voltage limits based on that\n");
        printf("Max mem DPM 0 MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 0 MVDD voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == memvddcivoltorder0 - 1) {
        memvddcivoltlimitlower0 = atoi(bdefaults);
        memvddcivoltlimitupper0 = atoi(bdefaults);
        gdouble gval = memvddcivoltlimitupper0;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt0), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt0), gval);
        printf("Default mem DPM 0 VDDCI voltage value exists, calculating mem VDDCI voltage limits based on that\n");
        printf("Max mem DPM 0 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 0 VDDCI voltage upper limit: %g mV\n", gval);
      }
       else if(cur_def_line == memclockorder0 - 1) {
        memclocklimitlower0 = atoi(bdefaults);
        memclocklimitupper0 = atoi(bdefaults);
        gdouble gval = memclocklimitupper0;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock0), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock0), gval);
        printf("Default mem DPM 0 VDDCI voltage value exists, calculating mem VDDCI voltage limits based on that\n");
        printf("Max mem DPM 0 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 0 VDDCI voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == memmvddvoltorder1 - 1) {
        memmvddvoltlimitlower1 = atoi(bdefaults);
        memmvddvoltlimitupper1 = atoi(bdefaults);
        gdouble gval = memmvddvoltlimitupper1;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt1), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt1), gval);
        printf("Default mem DPM 1 MVDD voltage value exists, calculating mem MVDD voltage limits based on that\n");
        printf("Max mem DPM 1 MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 1 MVDD voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == memvddcivoltorder1 - 1) {
        memvddcivoltlimitlower1 = atoi(bdefaults);
        memvddcivoltlimitupper1 = atoi(bdefaults);
        gdouble gval = memvddcivoltlimitupper1;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt1), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt1), gval);
        printf("Default mem DPM 1 VDDCI voltage value exists, calculating mem VDDCI voltage limits based on that\n");
        printf("Max mem DPM 1 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 1 VDDCI voltage upper limit: %g mV\n", gval);
      }
       else if(cur_def_line == memclockorder1 - 1) {
        memclocklimitlower1 = atoi(bdefaults);
        memclocklimitupper1 = atoi(bdefaults);
        gdouble gval = memclocklimitupper1;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock1), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock1), gval);
        printf("Default mem DPM 1 VDDCI voltage value exists, calculating mem VDDCI voltage limits based on that\n");
        printf("Max mem DPM 1 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 1 VDDCI voltage upper limit: %g mV\n", gval);
      }

      else if(cur_def_line == memmvddvoltorder2 - 1) {
        memmvddvoltlimitlower2 = atoi(bdefaults);
        memmvddvoltlimitupper2 = atoi(bdefaults);
        gdouble gval = memmvddvoltlimitupper2;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt2), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt2), gval);
        printf("Default mem DPM 2 MVDD voltage value exists, calculating mem MVDD voltage limits based on that\n");
        printf("Max mem DPM 2 MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 2 MVDD voltage upper limit: %g mV\n", gval);
      }
      else if(cur_def_line == memvddcivoltorder2 - 1) {
        memvddcivoltlimitlower2 = atoi(bdefaults);
        memvddcivoltlimitupper2 = atoi(bdefaults);
        gdouble gval = memvddcivoltlimitupper2;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt2), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt2), gval);
        printf("Default mem DPM 2 VDDCI voltage value exists, calculating mem VDDCI voltage limits based on that\n");
        printf("Max mem DPM 2 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 2 VDDCI voltage upper limit: %g mV\n", gval);
      }
       else if(cur_def_line == memclockorder2 - 1) {
        memclocklimitlower2 = atoi(bdefaults);
        memclocklimitupper2 = atoi(bdefaults);
        gdouble gval = memclocklimitupper2;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock2), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock2), gval);
        printf("Default mem DPM 2 VDDCI voltage value exists, calculating mem VDDCI voltage limits based on that\n");
        printf("Max mem DPM 2 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 2 VDDCI voltage upper limit: %g mV\n", gval);
      }
      cur_def_line++;
    }
    fclose(defaultvalues);
    }
    else {
      // default settings exist but are outdated or corrupt
      nodefault = 1;
    }
  }
  else {
  // no default settings, get the data from pp table  
  nodefault = 1;
  }

if (nodefault == 1) {
    FILE *fgpupower = popen(gpupower, "r");
    if(fgpupower) {
      if(fgets(bpower, sizeof bpower, fgpupower)){
        powerval = atoi(bpower);
        printf("No valid default power value exists, using value from pp table\n");
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
        printf("No valid default mem DPM 3 MVDD voltage value exists, using value from pp table\n");
        printf("Max mem DPM 3 MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 3 MVDD voltage upper limit: %g mV\n", gval);
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
        printf("No valid default mem DPM 3 VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem DPM 3 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 3 VDDCI voltage upper limit: %g mV\n", gval);
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
        printf("No valid default max SoC voltage value exists, using value from pp table\n");
        printf("Max SoC voltage lower limit: %g mV\n", gval);
        printf("Max SoC voltage upper limit: %g mV\n", gval);
      }
      pclose(fsocvolt);
    }

    FILE *fsocvoltmin = popen(socvoltmin, "r");
    if(fsocvoltmin) {
      if(fgets(bsocvoltmin, sizeof bsocvoltmin, fsocvoltmin)){
        socvoltminlimitlower = atoi(bsocvoltmin) / 4;
        socvoltminlimitupper = atoi(bsocvoltmin) / 4;
        gdouble gval = socvoltminlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvoltmin), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvoltmin), gval);
        printf("No valid default SoC min voltage value exists, using value from pp table\n");
        printf("Min SoC voltage lower limit: %g mV\n", gval);
        printf("Min SoC voltage upper limit: %g mV\n", gval);
      }
      pclose(fsocvoltmin);
    }

    FILE *fgfxvoltmin = popen(gfxvoltmin, "r");
    if(fgfxvoltmin) {
      if(fgets(bgfxvoltmin, sizeof bgfxvoltmin, fgfxvoltmin)){
        gfxvoltminlimitlower = atoi(bgfxvoltmin) / 4;
        gfxvoltminlimitupper = atoi(bgfxvoltmin) / 4;
        gdouble gval = gfxvoltminlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvoltmin), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvoltmin), gval);
        printf("No valid default Gfx min voltage value exists, using value from pp table\n");
        printf("Min Gfx voltage lower limit: %g mV\n", gval);
        printf("Min Gfx voltage upper limit: %g mV\n", gval);
      }
      pclose(fgfxvoltmin);
    }

    FILE *fsocclock = popen(socclock, "r");
    if(fsocclock) {
      if(fgets(bsocclock, sizeof bsocclock, fsocclock)){
        socclocklimitlower = atoi(bsocclock);
        socclocklimitupper = atoi(bsocclock);
        gdouble gval = socclocklimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), gval);
        printf("No valid default SoC clock value exists, using value from pp table\n");
        printf("Max SoC clock lower limit: %g MHz\n", gval);
        printf("Max SoC clock upper limit: %g MHz\n", gval);
      }
      pclose(fsocclock);
    }

     FILE *fvoltoffset = popen(voltoffset, "r");
      if(fvoltoffset) {
        if(fgets(bvoltoffset, sizeof bvoltoffset, fvoltoffset)){
        voltoffsetlimitlower = atof(bvoltoffset) * 1000;
        voltoffsetlimitupper = atof(bvoltoffset) * 1000;
        gdouble gval = voltoffsetlimitupper;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_voltoffset), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_voltoffset), gval);
        printf("No valid default static voltage offset value exists, using value from pp table\n");
        printf("Static voltage offset lower limit: %g mV\n", gval);
        printf("Static voltage offset upper limit: %g mV\n", gval);
        }
        pclose(fvoltoffset);
      }

      FILE *fmemmvddvolt0 = popen(memmvddvolt0, "r");
      if(fmemmvddvolt0) {
        if(fgets(bmemmvddvolt0, sizeof bmemmvddvolt0, fmemmvddvolt0)){
        memmvddvoltlimitlower0 = atoi(bmemmvddvolt0) / 4;
        memmvddvoltlimitupper0 = atoi(bmemmvddvolt0) / 4;
        gdouble gval = memmvddvoltlimitupper0;

        // TODO: user DPM 1 value as upper? and so on

        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt0), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt0), gval);
        printf("No valid default mem DPM 0 MVDD voltage value exists, using value from pp table\n");
        printf("Max mem DPM 0 MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 0 MVDD voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemmvddvolt0);
      }

      FILE *fmemvddcivolt0 = popen(memvddcivolt0, "r");
      if(fmemvddcivolt0) {
        if(fgets(bmemvddcivolt0, sizeof bmemvddcivolt0, fmemvddcivolt0)){
        memvddcivoltlimitlower0 = atoi(bmemvddcivolt0) / 4;
        memvddcivoltlimitupper0 = atoi(bmemvddcivolt0) / 4;
        gdouble gval = memvddcivoltlimitupper0;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt0), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt0), gval);
        printf("No valid default mem DPM 0 VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem DPM 0 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 0 VDDCI voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemvddcivolt0);
      }

      FILE *fmemclock0 = popen(memclock0, "r");
      if(fmemclock0) {
        if(fgets(bmemclock0, sizeof bmemclock0, fmemclock0)){
        memclocklimitlower0 = atoi(bmemclock0);
        memclocklimitupper0 = atoi(bmemclock0);
        gdouble gval = memclocklimitupper0;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock0), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock0), gval);
        printf("No valid default mem DPM 0 VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem DPM 0 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 0 VDDCI voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemclock0);
      }

      FILE *fmemmvddvolt1 = popen(memmvddvolt1, "r");
      if(fmemmvddvolt1) {
        if(fgets(bmemmvddvolt1, sizeof bmemmvddvolt1, fmemmvddvolt1)){
        memmvddvoltlimitlower1 = atoi(bmemmvddvolt1) / 4;
        memmvddvoltlimitupper1 = atoi(bmemmvddvolt1) / 4;
        gdouble gval = memmvddvoltlimitupper1;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt1), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt1), gval);
        printf("No valid default mem DPM 1 MVDD voltage value exists, using value from pp table\n");
        printf("Max mem DPM 1 MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 1 MVDD voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemmvddvolt1);
      }

      FILE *fmemvddcivolt1 = popen(memvddcivolt1, "r");
      if(fmemvddcivolt1) {
        if(fgets(bmemvddcivolt1, sizeof bmemvddcivolt1, fmemvddcivolt1)){
        memvddcivoltlimitlower1 = atoi(bmemvddcivolt1) / 4;
        memvddcivoltlimitupper1 = atoi(bmemvddcivolt1) / 4;
        gdouble gval = memvddcivoltlimitupper1;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt1), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt1), gval);
        printf("No valid default mem DPM 1 VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem DPM 1 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 1 VDDCI voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemvddcivolt1);
      }

      FILE *fmemclock1 = popen(memclock1, "r");
      if(fmemclock1) {
        if(fgets(bmemclock1, sizeof bmemclock1, fmemclock1)){
        memclocklimitlower1 = atoi(bmemclock1);
        memclocklimitupper1 = atoi(bmemclock1);
        gdouble gval = memclocklimitupper1;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock1), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock1), gval);
        printf("No valid default mem DPM 1 VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem DPM 1 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 1 VDDCI voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemclock1);
      }

      FILE *fmemmvddvolt2 = popen(memmvddvolt2, "r");
      if(fmemmvddvolt2) {
        if(fgets(bmemmvddvolt2, sizeof bmemmvddvolt2, fmemmvddvolt2)){
        memmvddvoltlimitlower2 = atoi(bmemmvddvolt2) / 4;
        memmvddvoltlimitupper2 = atoi(bmemmvddvolt2) / 4;
        gdouble gval = memmvddvoltlimitupper2;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt2), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt2), gval);
        printf("No valid default mem DPM 2 MVDD voltage value exists, using value from pp table\n");
        printf("Max mem DPM 2 MVDD voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 2 MVDD voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemmvddvolt2);
      }

      FILE *fmemvddcivolt2 = popen(memvddcivolt2, "r");
      if(fmemvddcivolt2) {
        if(fgets(bmemvddcivolt2, sizeof bmemvddcivolt2, fmemvddcivolt2)){
        memvddcivoltlimitlower2 = atoi(bmemvddcivolt2) / 4;
        memvddcivoltlimitupper2 = atoi(bmemvddcivolt2) / 4;
        gdouble gval = memvddcivoltlimitupper2;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt2), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt2), gval);
        printf("No valid default mem DPM 2 VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem DPM 2 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 2 VDDCI voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemvddcivolt2);
      }

      FILE *fmemclock2 = popen(memclock2, "r");
      if(fmemclock2) {
        if(fgets(bmemclock2, sizeof bmemclock2, fmemclock2)){
        memclocklimitlower2 = atoi(bmemclock2);
        memclocklimitupper2 = atoi(bmemclock2);
        gdouble gval = memclocklimitupper2;
        gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock2), gval);
        gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock2), gval);
        printf("No valid default mem DPM 2 VDDCI voltage value exists, using value from pp table\n");
        printf("Max mem DPM 2 VDDCI voltage lower limit: %g mV\n", gval);
        printf("Max mem DPM 2 VDDCI voltage upper limit: %g mV\n", gval);
        }
        pclose(fmemclock2);
      }
}

// set limits to adjustments for those who have pp table limits
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
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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

  FILE *fgfxvoltmin = popen(gfxvoltmin, "r");
  if(fgfxvoltmin) {
  while(fgets(bdata, sizeof bdata, fgfxvoltmin)){
    int quarterval = atoi(bdata) / 4;
    gdouble val = quarterval;
    if(quarterval > 0) {
      printf("Min Gfx voltage: %g mV\n", val);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxvoltmin),TRUE); 
      if (gfxvoltminlimitlower > quarterval || quarterval > gfxvoltminlimitupper) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
      }
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvoltmin), val);
    }
    else {
      printf("Min Gfx voltage: Error, 0 or no value\n");
      gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_gfxvoltmin), "Error");
      error = 1;
      break;
    }     
  }
  pclose(fgfxvoltmin);
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
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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
        printf("Max mem DPM 3 MVDD voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt),TRUE); 
        if (memmvddvoltlimitlower > quarterval || quarterval > memmvddvoltlimitupper) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt), val);
      }
      else {
        printf("Max mem DPM 3 MVDD voltage: Error, 0 or no value\n");
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
        printf("Max mem DPM 3 VDDCI voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt),TRUE); 
        if (memvddcivoltlimitlower > quarterval || quarterval > memvddcivoltlimitupper) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt), val);
      }
      else {
        printf("Max mem DPM 3 VDDCI voltage: Error, 0 or no value\n");
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
      printf("Max mem DPM 3 clock: %d MHz\n", ival);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock),TRUE);
      if (memclocklimitlower > ival || ival > memclocklimitupper) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
      } 
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock), gval); 
    }
    else {
      printf("Max mem DPM 3 clock: Error, 0 or no value\n");
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
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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

  FILE *fsocvoltmin = popen(socvoltmin, "r");
    if(fsocvoltmin) {
    while(fgets(bdata, sizeof bdata, fsocvoltmin)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Min SoC voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socvoltmin),TRUE); 
        if (socvoltminlimitlower > quarterval || quarterval > socvoltminlimitupper) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvoltmin), val);
      }
      else {
        printf("Min SoC voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_socvoltmin), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fsocvoltmin);
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
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
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

  FILE *fvoltoffset = popen(voltoffset, "r");
    if(fvoltoffset) {
    while(fgets(bdata, sizeof bdata, fvoltoffset)){
      float fval = atof(bdata) * 1000;
      gdouble val = fval;
      printf("Static voltage offset: %g mV\n", val);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_voltoffset),TRUE); 
      if (voltoffsetlimitlower > fval || fval > voltoffsetlimitupper) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
      }
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_voltoffset), val);
      }     
  pclose(fvoltoffset);
  }

  // DPM 0 

   FILE *fmemmvddvolt0 = popen(memmvddvolt0, "r");
    if(fmemmvddvolt0) {
    while(fgets(bdata, sizeof bdata, fmemmvddvolt0)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max mem DPM 0 MVDD voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt0),TRUE); 
        if (memmvddvoltlimitlower0 > quarterval || quarterval > memmvddvoltlimitupper0) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt0), val);
      }
      else {
        printf("Max mem DPM 0 MVDD voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memmvddvolt0), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fmemmvddvolt0);
  }

  FILE *fmemvddcivolt0 = popen(memvddcivolt0, "r");
    if(fmemvddcivolt0) {
    while(fgets(bdata, sizeof bdata, fmemvddcivolt0)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max mem DPM 0 VDDCI voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt0),TRUE); 
        if (memvddcivoltlimitlower0 > quarterval || quarterval > memvddcivoltlimitupper0) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt0), val);
      }
      else {
        printf("Max mem DPM 0 VDDCI voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memvddcivolt0), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fmemvddcivolt0);
  }


  FILE *fmemclock0 = popen(memclock0, "r");
  if(fmemclock0) {

  while(fgets(bdata, sizeof bdata, fmemclock0)){
    int ival = atoi(bdata);
    gdouble gval = ival;
    if(ival > 0) {
      printf("Max mem DPM 0 clock: %d MHz\n", ival);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock0),TRUE);
      if (memclocklimitlower0 > ival || ival > memclocklimitupper0) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
      } 
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock0), gval); 
    }
    else {
      printf("Max mem DPM 0 clock: Error, 0 or no value\n");
      gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memclock0), "Error");
      error = 1;
      break;
    }     
  }
  pclose(fmemclock0);
  }

  // DPM 1

    FILE *fmemmvddvolt1 = popen(memmvddvolt1, "r");
    if(fmemmvddvolt1) {
    while(fgets(bdata, sizeof bdata, fmemmvddvolt1)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max mem DPM 1 MVDD voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt1),TRUE); 
        if (memmvddvoltlimitlower1 > quarterval || quarterval > memmvddvoltlimitupper1) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt1), val);
      }
      else {
        printf("Max mem DPM 1 MVDD voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memmvddvolt1), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fmemmvddvolt1);
  }

  FILE *fmemvddcivolt1 = popen(memvddcivolt1, "r");
    if(fmemvddcivolt1) {
    while(fgets(bdata, sizeof bdata, fmemvddcivolt1)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max mem DPM 1 VDDCI voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt1),TRUE); 
        if (memvddcivoltlimitlower1 > quarterval || quarterval > memvddcivoltlimitupper1) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt1), val);
      }
      else {
        printf("Max mem DPM 1 VDDCI voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memvddcivolt1), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fmemvddcivolt1);
  }


  FILE *fmemclock1 = popen(memclock1, "r");
  if(fmemclock1) {
  while(fgets(bdata, sizeof bdata, fmemclock1)){
    int ival = atoi(bdata);
    gdouble gval = ival;
    if(ival > 0) {
      printf("Max mem DPM 1 clock: %d MHz\n", ival);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock1),TRUE);
      if (memclocklimitlower1 > ival || ival > memclocklimitupper1) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
      } 
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock1), gval); 
    }
    else {
      printf("Max mem DPM 1 clock: Error, 0 or no value\n");
      gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memclock1), "Error");
      error = 1;
      break;
    }     
  }
  pclose(fmemclock1);
  }

  // DPM 2

  FILE *fmemmvddvolt2 = popen(memmvddvolt2, "r");
    if(fmemmvddvolt2) {
    while(fgets(bdata, sizeof bdata, fmemmvddvolt2)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max mem DPM 2 MVDD voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt2),TRUE); 
        if (memmvddvoltlimitlower2 > quarterval || quarterval > memmvddvoltlimitupper2) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt2), val);
      }
      else {
        printf("Max mem DPM 2 MVDD voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memmvddvolt2), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fmemmvddvolt2);
  }

  FILE *fmemvddcivolt2 = popen(memvddcivolt2, "r");
    if(fmemvddcivolt2) {
    while(fgets(bdata, sizeof bdata, fmemvddcivolt2)){
      int quarterval = atoi(bdata) / 4;
      gdouble val = quarterval;
      if(quarterval > 0) {
        printf("Max mem DPM 2 VDDCI voltage: %g mV\n", val);
        gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt2),TRUE); 
        if (memvddcivoltlimitlower2 > quarterval || quarterval > memvddcivoltlimitupper2) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt2), val);
      }
      else {
        printf("Max mem DPM 2 VDDCI voltage: Error, 0 or no value\n");
        gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memvddcivolt2), "Error");
        error = 1;
        break;
      }     
    }
  pclose(fmemvddcivolt2);
  }


  FILE *fmemclock2 = popen(memclock2, "r");
  if(fmemclock2) {

  while(fgets(bdata, sizeof bdata, fmemclock2)){
    int ival = atoi(bdata);
    gdouble gval = ival;
    if(ival > 0) {
      printf("Max mem DPM 2 clock: %d MHz\n", ival);
      gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock2),TRUE);
      if (memclocklimitlower2 > ival || ival > memclocklimitupper2) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
      } 
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock2), gval); 
    }
    else {
      printf("Max mem DPM 2 clock: Error, 0 or no value\n");
      gtk_entry_set_text (GTK_ENTRY (app_wdgts->g_edit_memclock2), "Error");
      error = 1;
      break;
    }     
  }
  pclose(fmemclock2);
  }
}

//remove temp dump file
if (remove(ftempname) == 0) {
  printf("Temp file %s deleted successfully\n", ftempname); 
}
else {
  printf("Unable to delete the temporary file %s\n", ftempname);
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
  if (goodtowrite == 1){
    // Fill data to values
    int igfxvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt));
    int igfxvoltmin = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvoltmin));
    int igpupower = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower));
    int igfxclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock));
    int imemmvddvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt));
    int imemvddcivolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt));
    int imemclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock));
    int isocvolt = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt));
    int isocvoltmin = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvoltmin));
    int isocclock = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock));
    float fvoltoffset = gtk_spin_button_get_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_voltoffset));
    int imemmvddvolt0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt0));
    int imemvddcivolt0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt0));
    int imemclock0 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock0));
    int imemmvddvolt1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt1));
    int imemvddcivolt1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt1));
    int imemclock1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock1));
    int imemmvddvolt2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt2));
    int imemvddcivolt2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt2));
    int imemclock2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock2));

    // save to files
    printf("Saving current values in %s and limits in %s as defaults\n", valuespath, limitspath);
    
    FILE *defaultvalues = fopen(valuespath, "w+");
    // here the order is important !!!
    fprintf(defaultvalues, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%f\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",\
    igfxvolt, igfxclock, igpupower, imemmvddvolt,\
    imemvddcivolt, imemclock, isocvolt, isocclock,\
    fvoltoffset, imemmvddvolt0, imemvddcivolt0, imemclock0,\
    imemmvddvolt1, imemvddcivolt1, imemclock1,\
    imemmvddvolt2, imemvddcivolt2, imemclock2,\
    igfxvoltmin, isocvoltmin\
    );

    FILE *defaultlimits = fopen(limitspath, "w+");
    // here the order is important !!!
    fprintf(defaultlimits, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%f\n%f\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",\
    gfxvoltlimitlower, gfxvoltlimitupper,\
    gfxclocklimitlower, gfxclocklimitupper,\
    gpupowerlimitlower, gpupowerlimitupper,\
    memmvddvoltlimitlower, memmvddvoltlimitupper,\
    memvddcivoltlimitlower, memvddcivoltlimitupper,\
    memclocklimitlower, memclocklimitupper,\
    socvoltlimitlower, socvoltlimitupper,\
    socclocklimitlower, socclocklimitupper,\
    voltoffsetlimitlower, voltoffsetlimitupper,\
    memmvddvoltlimitlower0, memmvddvoltlimitupper0,\
    memvddcivoltlimitlower0, memvddcivoltlimitupper0,\
    memclocklimitlower0, memclocklimitupper0,\
    memmvddvoltlimitlower1, memmvddvoltlimitupper1,\
    memvddcivoltlimitlower1, memvddcivoltlimitupper1,\
    memclocklimitlower1, memclocklimitupper1,\
    memmvddvoltlimitlower2, memmvddvoltlimitupper2,\
    memvddcivoltlimitlower2, memvddcivoltlimitupper2,\
    memclocklimitlower2, memclocklimitupper2,\
    gfxvoltminlimitlower, gfxvoltminlimitupper,\
    socvoltminlimitlower, socvoltminlimitupper\
    );
    
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