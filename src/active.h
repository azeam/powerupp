#ifndef ACTIVE_H_
#define ACTIVE_H_

int set_limits_from_pp_table();
int set_values_from_pp_table(app_widgets *);
int save_defaults_settings(app_widgets *);
const char *values_to_keyfile(app_widgets *);

#endif