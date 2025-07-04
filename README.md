# Tokyobash
A custom bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com) for the penguin and git icons.
## Features

#### Highlights
Highlights the current directory and changes color depending on whether you're in HOME/, root/, or /mnt.
  
![tokyobash_01](https://github.com/user-attachments/assets/f8f1f02f-abf7-4a15-b024-491dcbb8675f)

##

#### Abbreviate Path
Abbreviates paths longer than 50 characters.

![tokyobash_02](https://github.com/user-attachments/assets/b3b94ac8-b229-442d-a684-4afb493649dc)

##

#### Git Branch Name
If current directory is a repository it will display the current branch name:
  
![tokyobash_03](https://github.com/user-attachments/assets/7a47c858-b828-4b0e-96d9-bff82d4d7d48)

##

#### Git Status Bar
An icon bar that shows changes to current repository.

  ![tokyobash_statusbar](https://github.com/user-attachments/assets/a94962ee-7279-4cc3-acd5-1e97b9a74e4a)

Displays the number of untracked files, files with unstaged changes, staged changes, number of commits, and if any commits available from remote. The latter will only fetch commits if repo has not been updated for 45 minutes.

The status bar will only display when there are changes to repo. And only show items that were changed. Is disabled by default, can be enabled via [Config](#config) setting.

<details>
  <summary>Icon Descriptions</summary>
  
  ![tokyobash_statusbarExp](https://github.com/user-attachments/assets/f3cce450-e44f-46aa-a4a2-b75f0145a9d5)
  
</details>
  

##

#### Themes
You can currently choose between 3 different themes.

Tokyonight:
  
![tokyobash_03](https://github.com/user-attachments/assets/75182622-9ce7-43a4-9fd0-d9f431105b26)

Catppuccin:

![tokyobash_catppuccin](https://github.com/user-attachments/assets/b99bbb0f-fe02-4c12-906b-51229efa3cb4)

Kanagawa:

![tokyobash_kanagawa](https://github.com/user-attachments/assets/3bc22047-44d2-4674-a20d-01537a94b5d7)

## Installation

The easiest way would be to have make and gcc or clang installed on your system.

In the root of the tokyobash repository, there are 3 commands to use:

  -`make` Will compile the tokyobash binary and place it in `repo_location/tokyobash/bin`.

  -`make install` Will compile tokyobash and copy the config file to `~/.config/tokyobash` if one is not already present.

  -`make install prefix=custom/path` Will compile tokyobash, copy config file, then copy tokyobash to `prefix/bin`.

#### No Make

If you dont have make, use these gcc commands:
```
gcc -c src/tokyobash.c
gcc -c src/lib/tokyobashlib.c
gcc -O3 tokyobash.o tokyobashlib.o -o tokyobash
```

For clang:

```
clang -c src/tokyobash.c
clang -c src/lib/tokyobashlib.c
clang -O3 tokyobash.o tokyobashlib.o -o tokyobash
```

You will need to manually create the directory tokyobash/  in `$XDG_CONFIG_HOME` if it's set. If not, create it in `~/.config` and copy the config file there.
Path to config file should be `~/.config/tokyobash/config`.

The config file is REQUIRED and tokyobash will NOT WORK without it!

##

Now add this to your .bashrc or .bash_profile:

```bash

export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary)"'

# If tokyobash was placed somewhere that's in your $PATH,
# then your PROMPT_COMMAND can look like this.

export PROMPT_COMMAND='PS1="$(tokyobash)"'

```
Now replace `path/to/tokyobash/binary` above with the acutal path to the compiled tokyobash binary.

Save then restart your terminal!

## Config

Config file uses standard key=value pairs.

`#` works like `//` in C for commenting.
Spaces, single, and double quotes are ignored.

`1 = enabled` `0 = disabled`

Default config:
```
theme       = tokyonight
#theme      = catppuccin
#theme      = kanagawa

git         = 1 # Turns all git integration on/off
branchname  = 1 # Turn the display of branch name on/off
statusbar   = 0 # Turn git status bar on/off
fetchtimer  = 1h # See below.
```

Setting git = 0 disables all git integration. This supercedes the branchname and statusbar settings.
So, if branchname = 1, and statusbar = 1. While git is set to 0, then neither branchname or statusbar are displayed.

fetchtimer will accept `#d`, `#h`, OR `#m`. d = days (1-29) h = hours (1-23) and m = minutes (1-59).

For example:

If `fetchtimer = 24m`, and `stat .git/FETCH_HEAD` reports that its been over 24 minutes since last update, fetch will be called.

Currently hours don't work quite right. For example if you have fetchtimer set to 3h. Meaning you want to update every 3 hours. And you last updated at 11:30pm. After midnight,
the date change will cause the repo to update again, but should work as expected afterwards.

##

### Disclaimer
I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.
So if I'm doing somthing stupid (I'm sure I am somewhere) please let me know.

All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme
