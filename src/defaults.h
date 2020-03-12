#ifndef DEFAULTS_H_
#define DEFAULTS_H_

int read_default_limits(char *, const char *, const char *, const char *);
int read_default_values(char *, const char *, const char *, const char *);
int set_default_limits(char *);
int set_default_values(char *, app_widgets *);
void on_btn_defaults_clicked(GtkButton *, app_widgets *);

#endif