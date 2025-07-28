#ifndef COLORS_H
#define COLORS_H

char bold[] = "\\[\\e[1m\\]";
char reset[] = "\\[\\e[00m\\]";
char red[] = "\\[\\e[38;2;255;77;77m\\]";
char desat_firebrick[] = "\\[\\e[38;2;191;72;72m\\]";
char pink[] = "\\[\\e[38;5;211m\\]";
char rose[] = "\\[\\e[38;5;217m\\]";
char peach[] = "\\[\\e[38;5;223m\\]";
char dark_orange[] = "\\[\\e[38;2;255;149;20m\\]";
char orange[] = "\\[\\e[38;5;214m\\]";
char yellow[] = "\\[\\e[38;2;255;255;0m\\]";
char green[] = "\\[\\e[38;2;0;255;0m\\]";
char lime_green[] = "\\[\\e[38;2;172;255;47m\\]";
char desat_lime[] = "\\[\\e[38;2;117;156;38m\\]";
char cyan[] = "\\[\\e[38;5;86m\\]";
char blue[] = "\\[\\e[38;5;4m\\]";
char sky_blue[] = "\\[\\e[38;5;117m\\]";
char teal[] = "\\[\\e[38;5;37m\\]";
char orchid[] = "\\[\\e[38;2;166;121;210m\\]";
char light_purple[] = "\\[\\e[38;5;182m\\]";
char purple[] = "\\[\\e[38;2;153;102;204m\\]";
char khaki[] = "\\[\\e[38;2;238;232;170m\\]";
char white[] = "\\[\\e[38;2;255;255;255m\\]";
char black[] = "\\[\\e[0;30m\\]";
char beige[] = "\\[\\e[38;2;239;239;200m\\]";

char blue_bg[] = "\\[\\e[48;5;4m\\]";
char sky_blue_bg[] = "\\[\\e[48;5;117m\\]";
char cyan_bg[] = "\\[\\e[48;5;86m\\]";
char teal_bg[] = "\\[\\e[48;5;37m\\]";

char* usr_color = &reset[0];
char* time_color = reset;
char* path_color = reset;
char* mnt_color = reset;
char* root_color = reset;

char* color_untracked = yellow;
char* color_unstaged = reset;
char* color_staged = reset;
char* color_committed = reset;
char* color_fetched = reset;

char* txt_fg_color = reset;
char* usr_bg = reset;
char* time_bg = reset;
char* branch_bg = reset;
char* path_bg = reset;
char* mnt_bg = reset;
char* root_bg = reset;
char* usr_icon_color = reset;
char* time_icon_color = reset;
char* branch_icon_color = reset;
char* path_icon_color = reset;
char* mnt_icon_color = reset;
char* root_icon_color = reset;

#endif
