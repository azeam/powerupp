#include <gtk/gtk.h>
#include "variables.h"

int read_limits_from_file(char *settingspath, const char *key, const char *keydesc, const char *keyunit) {
  g_autoptr(GError) error = NULL;
  g_autoptr(GKeyFile) key_file = g_key_file_new ();
  int ival = -10000;

  if (!g_key_file_load_from_file (key_file, settingspath, G_KEY_FILE_NONE, &error)) {
    if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
      printf("Error loading settings file %s\n", settingspath);
    }
  }
  else {
    // don't throw error if key is not found, this should make it easier to implement cards with lower number of settings    
    if (!g_key_file_has_key (key_file, "Limits", key, &error)) {
      if (!g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
          printf("No saved %s found\n", keydesc);
      }
    }
    else {
      ival = g_key_file_get_integer(key_file, "Limits", key, &error);
      if (!g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
        printf("Saved %s: %d %s\n", keydesc, ival, keyunit);
      }
    }
  }
  g_key_file_free (key_file);
  return ival;
}

int read_values_from_file(char *settingspath, const char *key, const char *keydesc, const char *keyunit) {
  g_autoptr(GError) error = NULL;
  g_autoptr(GKeyFile) key_file = g_key_file_new ();
  int ival = -10000;

  if (!g_key_file_load_from_file (key_file, settingspath, G_KEY_FILE_NONE, &error)) {
    if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
      printf("Error loading settings file %s\n", settingspath);
    }
  }
  else {
    // don't throw error if key is not found, just count the numbers set per card, this should make it easier to implement cards with lower number of settings    
      if (!g_key_file_has_key (key_file, "Values", key, &error)) {
        if (!g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
            printf("No saved %s found\n", keydesc);
        }
      }
      else {
        ival = g_key_file_get_integer(key_file, "Values", key, &error);
        if (!g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
          printf("Saved %s: %d %s\n", keydesc, ival, keyunit);
        }
      }
  }
  g_key_file_free (key_file);
  return ival;
}

int set_limits_from_file(char *settingspath) {
  g_autoptr(GKeyFile) key_file = g_key_file_new ();
  g_autoptr(GError) error = NULL;
  gsize deflimitslength;
  int readerror = 0;

  // load defaults file
  if(access(settingspath, F_OK) != -1) { 
    if (!g_key_file_load_from_file (key_file, settingspath, G_KEY_FILE_NONE, &error)) {
      if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
        printf("Error loading settings file\n");
        readerror = 1;
      }
    }
    else {
      // default settings file exist, count the number of settings in it
      g_key_file_get_keys (key_file, "Limits", &deflimitslength, &error);
      //load defaults if files contain the values they should
      if (deflimitslength == numberoflimits) {
        gint ival;
        
        ival = read_limits_from_file(settingspath, "gfxvoltminlimitlower", "lower min Gfx voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvoltmin), ival);
          gfxvoltminlimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "gfxvoltminlimitupper", "upper min Gfx voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvoltmin), ival);
          gfxvoltminlimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "gfxvoltlimitlower", "lower max Gfx voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvolt), ival);
          gfxvoltlimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "gfxvoltlimitupper", "upper max Gfx voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvolt), ival);
          gfxvoltlimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "gpupowerlimitlower", "lower GPU power limit", "W");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gpupower), ival);
          gpupowerlimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "gpupowerlimitupper", "upper GPU power limit", "W");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gpupower), ival);
          gpupowerlimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "gfxclocklimitlower", "lower Gfx clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxclock), ival);
          gfxclocklimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "gfxclocklimitupper", "upper Gfx clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxclock), ival);
          gfxclocklimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "socvoltminlimitlower", "lower min SoC voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvoltmin), ival);
          socvoltminlimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "socvoltminlimitupper", "upper min SoC voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvoltmin), ival);
          socvoltminlimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "socvoltlimitlower", "lower max SoC voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), ival);
          socvoltlimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "socvoltlimitupper", "upper max SoC voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), ival);
          socvoltlimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "socclocklimitlower", "lower SoC clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), ival);
          socclocklimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "socclocklimitupper", "upper SoC clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), ival);
          socclocklimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "voltoffsetlimitlower", "lower static voltage offset limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_voltoffset), ival);
          voltoffsetlimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "voltoffsetlimitupper", "upper static voltage offset limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_voltoffset), ival);
          voltoffsetlimitupper = ival;
        }

        // DPM 0
        ival = read_limits_from_file(settingspath, "memmvddvoltlimitlower0", "lower mem DPM 0 MVDD voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt0), ival);
          memmvddvoltlimitlower0 = ival;
        }
        ival = read_limits_from_file(settingspath, "memmvddvoltlimitupper0", "upper mem DPM 0 MVDD voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt0), ival);
          memmvddvoltlimitupper0 = ival;
        }

        ival = read_limits_from_file(settingspath, "memvddcivoltlimitlower0", "lower mem DPM 0 VDDCI voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt0), ival);
          memvddcivoltlimitlower0 = ival;
        }
        ival = read_limits_from_file(settingspath, "memvddcivoltlimitupper0", "upper mem DPM 0 VDDCI voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt0), ival);
          memvddcivoltlimitupper0 = ival;
        }

        ival = read_limits_from_file(settingspath, "memclocklimitlower0", "lower mem DPM 0 clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock0), ival);
          memclocklimitlower0 = ival;
        }
        ival = read_limits_from_file(settingspath, "memclocklimitupper0", "upper mem DPM 0 clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock0), ival);
          memclocklimitupper0 = ival;
        }

        // DPM 1
        ival = read_limits_from_file(settingspath, "memmvddvoltlimitlower1", "lower mem DPM 1 MVDD voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt1), ival);
          memmvddvoltlimitlower1 = ival;
        }
        ival = read_limits_from_file(settingspath, "memmvddvoltlimitupper1", "upper mem DPM 1 MVDD voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt1), ival);
          memmvddvoltlimitupper1 = ival;
        }

        ival = read_limits_from_file(settingspath, "memvddcivoltlimitlower1", "lower mem DPM 1 VDDCI voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt1), ival);
          memvddcivoltlimitlower1 = ival;
        }
        ival = read_limits_from_file(settingspath, "memvddcivoltlimitupper1", "upper mem DPM 1 VDDCI voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt1), ival);
          memvddcivoltlimitupper1 = ival;
        }

        ival = read_limits_from_file(settingspath, "memclocklimitlower1", "lower mem DPM 1 clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock1), ival);
          memclocklimitlower1 = ival;
        }
        ival = read_limits_from_file(settingspath, "memclocklimitupper1", "upper mem DPM 1 clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock1), ival);
          memclocklimitupper1 = ival;
        }

        // DPM 2
        ival = read_limits_from_file(settingspath, "memmvddvoltlimitlower2", "lower mem DPM 2 MVDD voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt2), ival);
          memmvddvoltlimitlower2 = ival;
        }
        ival = read_limits_from_file(settingspath, "memmvddvoltlimitupper2", "upper mem DPM 2 MVDD voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt2), ival);
          memmvddvoltlimitupper2 = ival;
        }

        ival = read_limits_from_file(settingspath, "memvddcivoltlimitlower2", "lower mem DPM 2 VDDCI voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt2), ival);
          memvddcivoltlimitlower2 = ival;
        }
        ival = read_limits_from_file(settingspath, "memvddcivoltlimitupper2", "upper mem DPM 2 VDDCI voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt2), ival);
          memvddcivoltlimitupper2 = ival;
        }

        ival = read_limits_from_file(settingspath, "memclocklimitlower2", "lower mem DPM 2 clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock2), ival);
          memclocklimitlower2 = ival;
        }
        ival = read_limits_from_file(settingspath, "memclocklimitupper2", "upper mem DPM 2 clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock2), ival);
          memclocklimitupper2 = ival;
        }

        // DPM 3
        ival = read_limits_from_file(settingspath, "memmvddvoltlimitlower", "lower mem DPM 3 MVDD voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), ival);
          memmvddvoltlimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "memmvddvoltlimitupper", "upper mem DPM 3 MVDD voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), ival);
          memmvddvoltlimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "memvddcivoltlimitlower", "lower mem DPM 3 VDDCI voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), ival);
          memvddcivoltlimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "memvddcivoltlimitupper", "upper mem DPM 3 VDDCI voltage limit", "mV");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), ival);
          memvddcivoltlimitupper = ival;
        }

        ival = read_limits_from_file(settingspath, "memclocklimitlower", "lower mem DPM 3 clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock), ival);
          memclocklimitlower = ival;
        }
        ival = read_limits_from_file(settingspath, "memclocklimitupper", "upper mem DPM 3 clock limit", "MHz");
        if (ival != -10000) {
          gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock), ival);
          memclocklimitupper = ival;
        }
      }
      else {
        readerror = 1;
      }
    }
  }
  else {
    readerror = 1;
  }
  g_key_file_free (key_file);
  if (readerror == 1) {
    return 1;
  }
  else {
    return 0;
  }
}

int set_values_from_file(char *settingspath, app_widgets *app_wdgts) {
  g_autoptr(GKeyFile) key_file = g_key_file_new ();
  
  g_autoptr(GError) error = NULL;
  gsize defvalueslength;
  int readerror = 0;

  // load defaults file if it exists
  if(access(settingspath, R_OK) != -1) { 
    if (!g_key_file_load_from_file (key_file, settingspath, G_KEY_FILE_NONE, &error)) {
      if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
        printf("Error loading settings file\n");
        readerror = 1;
      }
    }
    else {
      // default settings file exist, count the number of settings in it
      g_key_file_get_keys (key_file, "Values", &defvalueslength, &error);
      //load defaults if files contain the values they should
      if (defvalueslength == numberofvalues) {
        gint ival;
        
        ival = read_values_from_file(settingspath, "gpupower", "max GPU power", "W");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gpupower),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_gpupower),TRUE);
          if (gpupowerlimitlower > ival || ival > gpupowerlimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower), ival);
        }

        ival = read_values_from_file(settingspath, "gfxvolt", "max Gfx voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxvolt),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_gfxvolt),TRUE);
          if (gfxvoltlimitlower > ival || ival > gfxvoltlimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt), ival);
        }

        ival = read_values_from_file(settingspath, "gfxclock", "max Gfx clock", "MHz");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxclock),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_gfxclock),TRUE);
          if (gfxclocklimitlower > ival || ival > gfxclocklimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock), ival);
        }

        ival = read_values_from_file(settingspath, "socvolt", "max SoC voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socvolt),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_socvolt),TRUE);
          if (socvoltlimitlower > ival || ival > socvoltlimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt), ival);
        }

        ival = read_values_from_file(settingspath, "socclock", "max SoC clock", "MHz");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socclock),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_socclock),TRUE);
          if (socclocklimitlower > ival || ival > socclocklimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock), ival);
        }

        ival = read_values_from_file(settingspath, "voltoffset", "static voltage offset", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_voltoffset),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_voltoffset),TRUE);
          if (voltoffsetlimitlower > ival || ival > voltoffsetlimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_voltoffset), ival);
        }

        ival = read_values_from_file(settingspath, "gfxvoltmin", "min Gfx voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_gfxvoltmin),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_gfxvoltmin),TRUE);
          if (gfxvoltminlimitlower > ival || ival > gfxvoltminlimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvoltmin), ival);
        }

        ival = read_values_from_file(settingspath, "socvoltmin", "min SoC voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_socvoltmin),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_socvoltmin),TRUE);
          if (socvoltminlimitlower > ival || ival > socvoltminlimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvoltmin), ival);
        }

        // DPM 0
        ival = read_values_from_file(settingspath, "memmvddvolt0", "max mem DPM 0 MVDD voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt0),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memmvddvolt0),TRUE);
          if (memmvddvoltlimitlower0 > ival || ival > memmvddvoltlimitupper0) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt0), ival);
        }

        ival = read_values_from_file(settingspath, "memvddcivolt0", "max mem DPM 0 VDDCI voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt0),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memvddcivolt0),TRUE);
          if (memvddcivoltlimitlower0 > ival || ival > memvddcivoltlimitupper0) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt0), ival);
        }

        ival = read_values_from_file(settingspath, "memclock0", "max mem DPM 0 clock", "MHz");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock0),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memclock0),TRUE);
          if (memclocklimitlower0 > ival || ival > memclocklimitupper0) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock0), ival);
        }

        // DPM 1
        ival = read_values_from_file(settingspath, "memmvddvolt1", "max mem DPM 1 MVDD voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt1),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memmvddvolt1),TRUE);
          if (memmvddvoltlimitlower1 > ival || ival > memmvddvoltlimitupper1) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt1), ival);
        }

        ival = read_values_from_file(settingspath, "memvddcivolt1", "max mem DPM 1 VDDCI voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt1),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memvddcivolt1),TRUE);
          if (memvddcivoltlimitlower1 > ival || ival > memvddcivoltlimitupper1) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt1), ival);
        }

        ival = read_values_from_file(settingspath, "memclock1", "max mem DPM 1 clock", "MHz");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock1),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memclock1),TRUE);
          if (memclocklimitlower1 > ival || ival > memclocklimitupper1) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock1), ival);
        }

        // DPM 2
        ival = read_values_from_file(settingspath, "memmvddvolt2", "max mem DPM 2 MVDD voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt2),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memmvddvolt2),TRUE);
          if (memmvddvoltlimitlower2 > ival || ival > memmvddvoltlimitupper2) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt2), ival);
        }

        ival = read_values_from_file(settingspath, "memvddcivolt2", "max mem DPM 2 VDDCI voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt2),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memvddcivolt2),TRUE);
          if (memvddcivoltlimitlower2 > ival || ival > memvddcivoltlimitupper2) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt2), ival);
        }

        ival = read_values_from_file(settingspath, "memclock2", "max mem DPM 2 clock", "MHz");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock2),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memclock2),TRUE);
          if (memclocklimitlower2 > ival || ival > memclocklimitupper2) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock2), ival);
        }

        //DPM 3
        ival = read_values_from_file(settingspath, "memmvddvolt", "max mem DPM 3 MVDD voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memmvddvolt),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memmvddvolt),TRUE);
          if (memmvddvoltlimitlower > ival || ival > memmvddvoltlimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt), ival);
        }

        ival = read_values_from_file(settingspath, "memvddcivolt", "max mem DPM 3 VDDCI voltage", "mV");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memvddcivolt),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memvddcivolt),TRUE);
          if (memvddcivoltlimitlower > ival || ival > memvddcivoltlimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt), ival);
        }

        ival = read_values_from_file(settingspath, "memclock", "max mem DPM 3 clock", "MHz");
        if (ival != -10000) {
          gtk_editable_set_editable(GTK_EDITABLE(app_wdgts->g_edit_memclock),TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_edit_memclock),TRUE);
          if (memclocklimitlower > ival || ival > memclocklimitupper) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), TRUE);
          }
          gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock), ival);
        }
      }
      else {
        readerror = 1;
        printf("Defaults file does not contain the expected number of settings\n");
      }
    }
  }
  else {
    readerror = 1;
    printf("Can't read defaults file at %s\n", defsettingspath);
  }
  g_key_file_free (key_file);
  if (readerror == 1) {
    return 1;
  }
  else {
    return 0;
  }
}

void on_opt_defaults_load_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts) {
  int readerror = 0;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_wdgts->g_toggle_limits), FALSE);

  if (set_limits_from_file(defsettingspath) == 1) {
    readerror = 1;
  }
  else {
    if (set_values_from_file(defsettingspath, app_wdgts) == 1) {
      readerror = 1;
    }
  }
  if (readerror == 1) {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Error loading default values, reload active", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_opt_persistent_save), FALSE);
  }
  else {
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "Default values loaded", -1);
    gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_opt_persistent_save), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(app_wdgts->g_opt_profile_save), TRUE);
  }
}
