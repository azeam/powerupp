#ifndef DEFAULTS_H_
#define DEFAULTS_H_

int read_default_limits(const char *, const char *, const char *);
int read_default_values(const char *, const char *, const char *);
int set_default_limits();
int set_default_values(app_widgets *);
void on_btn_defaults_clicked(GtkButton *, app_widgets *);

#endif