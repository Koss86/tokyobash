# Tokyobash
A custom bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com) for the penguin and git icons.
## Features
Highlights the current directory and changes color depending on whether you're in HOME/, root/, or /mnt.
<details>
<summary>Screenshot</summary>
  
![Screenshot_display](https://github.com/user-attachments/assets/f331bb4d-6a9d-4378-aa94-0f26bb2aed78)

</details>

If current directory is a repository it will display the current branch name:
<details>
  <summary>Screenshot</summary>
  
  ![tokyobash_tokyonight_1](https://github.com/user-attachments/assets/dd4d42ff-7daa-435f-8b42-457fc9c774a7)

</details>

And it will abbreviate paths longer than 50 characters.
<details>
<summary>Screenshot</summary>

![tokyobash_arbv](https://github.com/user-attachments/assets/f8e4d0a6-7ce0-46f4-946d-c786fd0d6933)

</details>

## Installation

Easiest way would be to have gcc and make installed.

In the root tokyobash directory, type:

  -`make`, will compile a tokyoBash binary and place it in repo_location/tokyobash/bin.

  -`make install`, will compile tokyobash and copy the config file to ~/.config/tokyobash if ones not already present.

  -`make install prefix=custom/path`, will compile tokyobash, copy config file, then copy tokyobash to prefix/bin.
As an example, when I build a new binary I use `make install prefix=$HOME/.local`

If you dont have make use these gcc commands:
```
gcc -O2 -c src/tokyobash.c
gcc -O2 -c src/lib/tokyobashlib.c
gcc -O2 tokyobash.o tokyobashlib.o -o tokyobash
```
You will have to manually create ~/.config/tokyobash and copy the config file from the repo to there.

The config file is REQUIRED and tokyobash will not work without it!

##

Now add this to your .bashrc or .bash_profile:

```bash

export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary)"'

```
Then replace 'path/to/tokyobash/binary' above with the acutal path to the compiled tokyobash binary.

Save then restart your terminal!

## Themes

You can currently choose between 3 different themes.

  Tokyonight:
  
  ![tokyobash_git](https://github.com/user-attachments/assets/339a549e-4c68-42ab-95f7-a5660e3ed322)
##

  Catppuccin:
  
![tokyobash_catppuccin_1](https://github.com/user-attachments/assets/f1619806-32bf-4364-936f-a3263b7dc8a2)

##

  Kanagawa:
  
  ![tokyobash_kanagawa_1](https://github.com/user-attachments/assets/9d026d34-54cc-4cbe-9be5-5ffc61cc9055)

To pick a theme, add `theme="theme_name"` somewhere above the `export PROMPT_COMMAND` line in your .bashrc or .bash_profile. Then pass it to the tokyobash binary as an argument, like so:
```bash
##Just for example, only choose one##
#For Tokyonight
theme="tokyonight"
#For Catppuccin
theme="catppuccin"
#For Kanagawa
theme="kanagawa"

export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary $SECONDS $theme)"'
```
If no theme is chosen, all text colors will be defualt.

##

### Disclaimer
I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.


All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme
