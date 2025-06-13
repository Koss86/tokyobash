# Tokyobash
A custom bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com) for the penguin icon.
## Features
Highlights the current directory and changes color depending on whether you're in HOME/, root/, or /mnt.

![tokyobash_1](https://github.com/user-attachments/assets/401314ad-97fa-4125-8ec3-8c07f5b321dc)

Also abbreviate paths longer than 50 characters.

![tokyobash_2](https://github.com/user-attachments/assets/380b6184-6d09-4616-8278-6d7c01b70cc0)


## Installation
You will need a C compiler. Either gcc or clang should be fine.

Navigate to the directory containing tokyobash.c in your terminal and type:
```
gcc -o tokyoBash tokyobash.c
```

If you're using clang, just replace gcc with clang and it will work the same.
This will create the binary whose path you'll need for the configuration below.

Now add this to your .bashrc or .bash_profile:

```bash
SECONDS=0

export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary $SECONDS)"'

alias clear='reset_and_clear'
reset_and_clear() {
  SECONDS=0
  command clear
}
```
Then replace 'path/to/tokyobash/binary' above with the acutal path to the compiled tokyobash binary.

Save then restart your terminal!
<details>
<summary>Remove blank line spacer</summary>
  
If you don't like the blank line spacer, you can remove the SECONDS variable from your .bashrc or .bash_profile file. It should work the same, just without the spacing.

SECONDS is only used for aesthetic reasons. I like the look of a blank line between each prompt. But, it has to be printed before the other printfs are called. If not, it will cause the input field to be below where intended. But printing it first leaves the issues of a blank line at the top upon opening, and the same when `clear` is used.

  In bash, SECONDS just adds 1 to itself every second the terminal is open. So we then know when the terminal has just opened, and we set up an alias to reset SECONDS when `clear` is called. Then we check if SECONDS is >1 to print the newline or not.

</details>

## Themes

You can currently choose between 3 different themes: [Tokyonight](https://github.com/user-attachments/assets/237e151e-a07f-4ed5-b4ef-f353a814b706), [Catppuccin](https://github.com/user-attachments/assets/a0b34a53-dde6-4e6b-8e0c-a1790469cf5a)
, and [Kanagawa](https://github.com/user-attachments/assets/fa380084-c2b2-4401-9777-2c15dad736b8).
To pick a theme, create a theme variable somewhere above the `export PROMPT_COMMAND` line in your .bashrc or .bash_profile. Then pass it to the tokyobash binary as an argument, like so:
```bash
#For Tokyonight
theme="tokyonight"
#For Catppuccin
theme="catppuccin"
#For Kanagawa
theme="kanagawa"

export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary $SECONDS $theme)"'
```
Tokyobash will default to Tokyonight even if no theme variable is created or passed. 

##

### Disclaimer
I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.

All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme
