# Tokyobash
A custom bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com) for the penguin and git icons.
## Features

#### Highlights
Highlights the current directory and changes color depending on whether you're in HOME/, root/, or /mnt.
<details>
<summary>Screenshot</summary>
  
![Screenshot_display](https://github.com/user-attachments/assets/f331bb4d-6a9d-4378-aa94-0f26bb2aed78)

</details>

##

#### Abbreviate Path
Abbreviates paths longer than 50 characters.
<details>
<summary>Screenshot</summary>

![tokyobash_arbv](https://github.com/user-attachments/assets/f8e4d0a6-7ce0-46f4-946d-c786fd0d6933)

</details>

##

#### Git Branch Name
If current directory is a repository it will display the current branch name:
<details>
  <summary>Screenshot</summary>
  
  ![tokyobash_tokyonight_1](https://github.com/user-attachments/assets/dd4d42ff-7daa-435f-8b42-457fc9c774a7)

</details>

##

#### Git Status Bar
An icon bar that shows changes to current repository.

<details>
  <summary>Screenshot</summary>

  ![tokyobash_statusbar](https://github.com/user-attachments/assets/a94962ee-7279-4cc3-acd5-1e97b9a74e4a)
  
</details>

Displays the number of untracked files, files with unstaged changes, staged changes, number of commits, and if any commits available from remote. The latter will only fetch commits if repo has not been updated for 45 minutes.

The status bar will only display when there are changes to repo. And only show items that were changed. Is disabled by default, can be enabled via [Config](#config) setting.

<details>
  <summary>Icon Descriptions</summary>
  
  ![tokyobash_statusbarExp](https://github.com/user-attachments/assets/f3cce450-e44f-46aa-a4a2-b75f0145a9d5)
  
</details>
  

##

#### Themes
You can currently choose between 3 different themes.
<details>
  <summary>Tokyonight</summary>

  ![tokyobash_git](https://github.com/user-attachments/assets/339a549e-4c68-42ab-95f7-a5660e3ed322)
  
</details>
<details>
  <summary>Catppuccin</summary>

  ![tokyobash_catppuccin_1](https://github.com/user-attachments/assets/f1619806-32bf-4364-936f-a3263b7dc8a2)
  
</details>

<details>
  <summary>Kanagawa</summary>

  ![tokyobash_kanagawa_1](https://github.com/user-attachments/assets/9d026d34-54cc-4cbe-9be5-5ffc61cc9055)
  
</details>

## Installation

The easiest way would be to have make and gcc or clang installed on your system.

In the root of the tokyobash repository, there are 3 commands to use:

  -`make` Will compile the tokyobash binary and place it in `repo_location/tokyobash/bin`.

  -`make install` Will compile tokyobash, generate the config file, then copy config to `~/.config/tokyobash` if one is not already present.

  -`make install prefix=custom/path` Will compile tokyobash, generate/copy config file, then copy tokyobash to `prefix/bin`.

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

Copy and paste this into your terminal to create the config directory and file:

```
mkdir ~/.config/tokyobash/ && printf 'theme = tokyonight\n#theme = catppuccin\n#theme = kanagawa\n\ngit = 1\nbranchname = 1\nstatusbar = 0\n' > ~/.config/tokyobash/config
```

Path to config file needs be `~/.config/tokyobash/config`.

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
```

Setting git = 0 disables all git integration. This supercedes the branchname and statusbar settings.
So, if branchname = 1, and statusbar = 1. While git is set to 0, then neither branchname or statusbar are displayed.

##

### Disclaimer
I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.
So if I'm doing somthing stupid (I'm sure I am somewhere) please let me know.

All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme
