#ifndef DEFAULTS_H_
#define DEFAULTS_H_

int read_limits_from_file(char *, const char *, const char *, const char *);
int read_default_values(char *, const char *, const char *, const char *);
int set_limits_from_file(char *);
int set_values_from_file(char *, app_widgets *);
void on_opt_defaults_load_activate(GtkButton *, app_widgets *);

#endif