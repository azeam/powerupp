#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include "variables.h"
#include "active.h"
#include "defaults.h"
#include "apply.h"
#include "save.h"

//testedit
// lost patience with glade, connecting manually
static void on_combobox_changed (GtkComboBoxText *combobox, gpointer user_data) {
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_active), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_defaults), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_perm), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(g_btn_apply), FALSE);

    if (gtk_combo_box_text_get_active_text (g_combobox) != 0) {
      card_num = gtk_combo_box_get_active(GTK_COMBO_BOX(g_combobox));
      gchar *card_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_combobox));
      char amd5700xt[512];
      snprintf(amd5700xt, sizeof(amd5700xt), "card %d: Navi 10 (Radeon 5xxx)", card_num);
      if (strcmp(card_text,amd5700xt) == 0) {
            
            // data for 5700 XT, possible to add data for other GPUs
            // get values
            snprintf(gfxvolt, sizeof(gfxvolt), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get smcPPTable/MaxVoltageGfx", card_num);
            snprintf(gfxclock, sizeof(gfxclock), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get smcPPTable/FreqTableGfx/1", card_num);
            snprintf(gpupower, sizeof(gpupower), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get smcPPTable/SocketPowerLimitAc/0", card_num);
            snprintf(memmvddvolt, sizeof(memmvddvolt), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get smcPPTable/MemMvddVoltage/3", card_num);
            snprintf(memvddcivolt, sizeof(memvddcivolt), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get smcPPTable/MemVddciVoltage/3", card_num);
            snprintf(memclock, sizeof(memclock), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get smcPPTable/FreqTableUclk/3", card_num);
            snprintf(socvolt, sizeof(socvolt), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get smcPPTable/MaxVoltageSoc", card_num);
            snprintf(socclock, sizeof(socclock), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get smcPPTable/FreqTableSocclk/1", card_num);

            //get limits
            snprintf(gfxvoltlimits, sizeof(gfxvoltlimits), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get OverDrive8Table/ODSettingsMax/3 && /usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get OverDrive8Table/ODSettingsMin/3", card_num, card_num);
            snprintf(gfxclocklimits, sizeof(gfxclocklimits), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get OverDrive8Table/ODSettingsMax/1 && /usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get OverDrive8Table/ODSettingsMin/1", card_num, card_num);
            snprintf(gpupowerlimits, sizeof(gpupowerlimits), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get OverDrive8Table/ODSettingsMax/9 && /usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get OverDrive8Table/ODSettingsMin/9", card_num, card_num);
            snprintf(memclocklimits, sizeof(memclocklimits), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get OverDrive8Table/ODSettingsMax/8 && /usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table get OverDrive8Table/ODSettingsMin/8", card_num, card_num);            

            //set up write commands
            //set "--write" at the end of cmd after testing or remove for testing
            snprintf(uppwrite, sizeof(uppwrite), "/usr/local/lib/powerupp/upp.py -i /sys/class/drm/card%d/device/pp_table set --write", card_num);
            gfxvoltset = "smcPPTable/MaxVoltageGfx=";
            gpupowerset = "smcPPTable/SocketPowerLimitAc/0=";
            gfxclockset = "smcPPTable/FreqTableGfx/1=";
            memmvddvoltset = "smcPPTable/MemMvddVoltage/3=";
            memvddcivoltset = "smcPPTable/MemVddciVoltage/3=";
            memclockset = "smcPPTable/FreqTableUclk/3=";
            socvoltset = "smcPPTable/MaxVoltageSoc=";
            socclockset = "smcPPTable/FreqTableSocclk/1=";

            //get user home directory
            p = getpwuid(getuid());
            if (p != NULL)
            {
              snprintf(dirname, sizeof(dirname),"/home/%s/.config/powerupp", p->pw_name);
              snprintf(valuespath, sizeof(valuespath), "%s/defaultvalues%d", dirname, card_num);
              snprintf(limitspath, sizeof(limitspath), "%s/defaultlimits%d", dirname, card_num);
            }
            else {
              printf("No user error\n");
            }

            gtk_widget_set_sensitive(GTK_WIDGET(g_btn_active), TRUE);
                if(access(valuespath, F_OK ) != -1 && access(limitspath, F_OK ) != -1) {
                  gtk_widget_set_sensitive(GTK_WIDGET(g_btn_defaults), TRUE);
                }
      }
      g_free(card_text);
    }
    
}

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    app_widgets     *widgets = g_slice_new(app_widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "/usr/local/lib/powerupp/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, widgets);

    // get pointers to the widgets
    // buttons
    g_btn_active = GTK_BUTTON(gtk_builder_get_object(builder, "btn_active"));
    g_btn_defaults = GTK_BUTTON(gtk_builder_get_object(builder, "btn_defaults"));
    g_btn_apply = GTK_BUTTON(gtk_builder_get_object(builder, "btn_apply"));
    g_btn_perm = GTK_BUTTON(gtk_builder_get_object(builder, "btn_perm"));
    g_btn_info = GTK_BUTTON(gtk_builder_get_object(builder, "btn_info"));
    
    // gpu select
    g_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combobox"));
    g_signal_connect (g_combobox, "changed", G_CALLBACK (on_combobox_changed), NULL);

    // spin buttons with their adjustments and toggles
    widgets->g_edit_gfxvolt = GTK_WIDGET(gtk_builder_get_object(builder, "edit_gfxvolt"));
    g_adj_gfxvolt = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_gfxvolt"));
    widgets->g_toggle_gfxvolt = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_gfxvolt"));

    widgets->g_edit_gfxclock = GTK_WIDGET(gtk_builder_get_object(builder, "edit_gfxclock"));
    g_adj_gfxclock = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_gfxclock"));
    widgets->g_toggle_gfxclock = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_gfxclock"));

    widgets->g_edit_gpupower = GTK_WIDGET(gtk_builder_get_object(builder, "edit_gpupower"));
    g_adj_gpupower = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_gpupower"));
    widgets->g_toggle_gpupower = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_gpupower"));

    widgets->g_edit_memmvddvolt = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memmvddvolt"));
    g_adj_memmvddvolt = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memmvddvolt"));
    widgets->g_toggle_memmvddvolt = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_memmvddvolt"));

    widgets->g_edit_memvddcivolt = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memvddcivolt"));
    g_adj_memvddcivolt = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memvddcivolt"));
    widgets->g_toggle_memvddcivolt = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_memvddcivolt"));

    widgets->g_edit_memclock = GTK_WIDGET(gtk_builder_get_object(builder, "edit_memclock"));
    g_adj_memclock = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_memclock"));
    widgets->g_toggle_memclock = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_memclock"));

    widgets->g_edit_socvolt = GTK_WIDGET(gtk_builder_get_object(builder, "edit_socvolt"));
    g_adj_socvolt = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_socvolt"));
    widgets->g_toggle_socvolt = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_socvolt"));

    widgets->g_edit_socclock = GTK_WIDGET(gtk_builder_get_object(builder, "edit_socclock"));
    g_adj_socclock = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adj_socclock"));
    widgets->g_toggle_socclock = GTK_WIDGET(gtk_builder_get_object(builder, "toggle_socclock"));

    widgets->g_revealer = GTK_WIDGET(gtk_builder_get_object(builder, "revealer"));
    g_text_revealer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "text_revealer"));
    widgets->g_textview_revealer = GTK_WIDGET(gtk_builder_get_object(builder, "textview_revealer"));

    // help guide
    widgets->g_guide = GTK_WIDGET(gtk_builder_get_object(builder, "guide"));

    // needs to be set before testing upp
    g_object_unref(builder);
    gtk_widget_show(window);                

    // test if UPP seems to work as it should, works but ugly workaround for not being able to read the output from python error with fgets
    FILE *ftestupptmp = popen("/usr/local/lib/powerupp/upp.py > /tmp/upperror 2>&1", "r");
    pclose(ftestupptmp);
    FILE *ftestupp = popen("cat /tmp/upperror", "r");
    char testupp[512];
    int upperror = 0;
    if(ftestupp != NULL) {
      while(fgets(testupp, sizeof testupp, ftestupp)){
      if(strstr(testupp,"ModuleNotFoundError") != NULL) {
        printf("UPP %s\n", testupp);
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "UPP error, missing dependencies", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
        upperror = 1;
        break;
      }
      else if (strstr(testupp,"not found") != NULL){
        printf("UPP %s\n", testupp);
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_text_revealer), "UPP error, file not found", -1);
        gtk_revealer_set_reveal_child (GTK_REVEALER(widgets->g_revealer), TRUE);
        upperror = 1;
        break;
      }
      }
    }
    pclose(ftestupp);

    if (upperror == 0) {
    //TODO: add more models
    char gpumodel[128];
    char amd5700xt[128] = "0x731f\n";
    int num = 0;
    char cardnum[128];
    //scan for GPUs and add them to combobox
    do {
        snprintf(cardnum, sizeof(cardnum), "/sys/class/drm/card%d/device/device", num);
        if (access(cardnum, F_OK) != -1) {
        printf("GPU %s exists\n", cardnum);

        FILE *model = fopen(cardnum, "r");
        if(fgets(gpumodel, sizeof gpumodel, model)){
          printf("GPU %s model is %s", cardnum, gpumodel);
          if (strcmp(gpumodel,amd5700xt) == 0) {
            char hgpumodel [128];
            snprintf(hgpumodel, sizeof(hgpumodel), "card %d: Navi 10 (Radeon 5xxx)", num);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(g_combobox), hgpumodel);
            gtk_combo_box_set_active(GTK_COMBO_BOX(g_combobox), num);
          }
          else {
            printf("Unsupported GPU\n");
            char hgpumodel [128];
            snprintf(hgpumodel, sizeof(hgpumodel), "card %d: Unsupported GPU", num);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(g_combobox), hgpumodel);
            gtk_combo_box_set_active(GTK_COMBO_BOX(g_combobox), num);
        }
        }
        fclose(model);
        num++;
        }
    } while (access(cardnum, F_OK) != -1);
    }
    
    gtk_main();
    g_slice_free(app_widgets, widgets);
    return 0;
}

void on_btn_revealer_clicked(GtkButton *button, app_widgets *app_wdgts) {
gtk_revealer_set_reveal_child (GTK_REVEALER(app_wdgts->g_revealer), FALSE);
}

void on_btn_info_clicked(GtkButton *button, app_widgets *app_wdgts) {
gtk_widget_show (GTK_WIDGET(app_wdgts->g_guide));
}

void on_guide_cancel(GtkButton *button, app_widgets *app_wdgts) {
gtk_widget_hide (GTK_WIDGET(app_wdgts->g_guide));
}

void on_guide_close(GtkButton *button, app_widgets *app_wdgts) {
gtk_widget_hide (GTK_WIDGET(app_wdgts->g_guide));
}

void on_toggle_gfxvolt_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
if (gtk_toggle_button_get_active(togglebutton)){
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvolt), 9999);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvolt), 0);
}
else {
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxvolt), gfxvoltlimitupper);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxvolt), gfxvoltlimitlower);

  //spin button should be updated if value is set higher than limit or it will stay above limit when toggling
  gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxvolt));
}
}

void on_toggle_gpupower_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
if (gtk_toggle_button_get_active(togglebutton)){
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gpupower), 9999);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gpupower), 0);
}
else {
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gpupower), gpupowerlimitupper);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gpupower), gpupowerlimitlower);

  gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_gpupower));
}
}

void on_toggle_gfxclock_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
if (gtk_toggle_button_get_active(togglebutton)){
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxclock), 9999);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxclock), 0);
}
else {
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_gfxclock), gfxclocklimitupper);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_gfxclock), gfxclocklimitlower);

  gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_gfxclock));
}
}

void on_toggle_memmvddvolt_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
if (gtk_toggle_button_get_active(togglebutton)){
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), 9999);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), 0);
}
else {
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memmvddvolt), memmvddvoltlimitupper);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memmvddvolt), memmvddvoltlimitlower);

  gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memmvddvolt));
}
}

void on_toggle_memvddcivolt_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
if (gtk_toggle_button_get_active(togglebutton)){
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), 9999);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), 0);
}
else {
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memvddcivolt), memvddcivoltlimitupper);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memvddcivolt), memvddcivoltlimitlower);

  gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memvddcivolt));
}
}

void on_toggle_memclock_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
if (gtk_toggle_button_get_active(togglebutton)){
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock), 9999);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock), 0);
}
else {
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_memclock), memclocklimitupper);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_memclock), memclocklimitlower);

  gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_memclock));
}
}

void on_toggle_socvolt_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
if (gtk_toggle_button_get_active(togglebutton)){
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), 9999);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), 0);
}
else {
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socvolt), socvoltlimitupper);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socvolt), socvoltlimitlower);

  gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_socvolt));
}
}

void on_toggle_socclock_toggled(GtkToggleButton *togglebutton, app_widgets *app_wdgts) {
if (gtk_toggle_button_get_active(togglebutton)){
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), 9999);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), 0);
}
else {
  gtk_adjustment_set_upper(GTK_ADJUSTMENT(g_adj_socclock), socclocklimitupper);
  gtk_adjustment_set_lower(GTK_ADJUSTMENT(g_adj_socclock), socclocklimitlower);

  gtk_spin_button_update(GTK_SPIN_BUTTON(app_wdgts->g_edit_socclock));
}
}

void on_window_main_destroy()
{
    gtk_main_quit();
}
