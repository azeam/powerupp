#ifndef ACTIVE_H_
#define ACTIVE_H_

int read_values_from_pp_table(char *, const char *, const char *);
int read_voltoffset_from_pp_table(char *, const char *, const char *);
int set_limits_from_pp_table();
int set_values_from_pp_table(app_widgets *);
int save_defaults_settings(app_widgets *);

#endif