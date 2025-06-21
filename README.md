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
  
  ![tokyobash_tokyonight_1](https://github.com/user-attachments/assets/edd76f0c-1de6-4d04-8d26-6bc3f3223e18)

</details>

And it will abbreviate paths longer than 50 characters.
<details>
<summary>Screenshot</summary>

![tokyobash_arbv](https://github.com/user-attachments/assets/f8e4d0a6-7ce0-46f4-946d-c786fd0d6933)

</details>

## Installation

Easiest way would be to have gcc and make on your system.
In the root tokyobash directory, just run `make`. This will
compile a tokyoBash binary in current directory. If you dont
have make use this gcc command:
```
gcc -O3 -o tokyoBash src/tokyobash.c src/lib/libtokyobash.c
```
Now add this to your .bashrc or .bash_profile:

```bash
theme="theme_name"

export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary $theme)"'

```
Then replace 'path/to/tokyobash/binary' above with the acutal path to the compiled tokyobash binary and 'theme_name' with the name of your chosen theme.

Save then restart your terminal!

## Themes

You can currently choose between 3 different themes.

  Tokyonight:
  
  ![tokyobash_git](https://github.com/user-attachments/assets/edd76f0c-1de6-4d04-8d26-6bc3f3223e18)
##

  Catppuccin:
  
![tokyobash_catppuccin_1](https://github.com/user-attachments/assets/920fc45a-e999-4b82-8607-56c4463f84d4)

##

  Kanagawa:
  
  ![tokyobash_kanagawa_1](https://github.com/user-attachments/assets/31b6bb5d-990e-4b1a-9100-8ca06d888c7d)

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
