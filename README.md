# Tokyobash
A custom bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com) for the penguin and git icons.
## Features
Highlights the current directory and changes color depending on whether you're in HOME/, root/, or /mnt.

![tokyobash_1](https://github.com/user-attachments/assets/401314ad-97fa-4125-8ec3-8c07f5b321dc)

Also, if current directory is a repository it will show the current branch name:
![tokyobash_git](https://github.com/user-attachments/assets/dd4d42ff-7daa-435f-8b42-457fc9c774a7)

And finally, it will abbreviate paths longer than 50 characters.

![tokyobash_2](https://github.com/user-attachments/assets/380b6184-6d09-4616-8278-6d7c01b70cc0)

## Themes

You can currently choose between 3 different themes.

  Tokyonight:
  
  ![tokyobash_git](https://github.com/user-attachments/assets/339a549e-4c68-42ab-95f7-a5660e3ed322)
##

  Catppuccin:

  ![tokyobash_catppuccin](https://github.com/user-attachments/assets/f1619806-32bf-4364-936f-a3263b7dc8a2)
##

  Kanagawa:
  
  ![tokyobash_kanagawa](https://github.com/user-attachments/assets/9d026d34-54cc-4cbe-9be5-5ffc61cc9055)



To pick a theme, add `theme="theme_name"` somewhere above the `export PROMPT_COMMAND` line in your .bashrc or .bash_profile. Then pass it to the tokyobash binary as an argument, like so:
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
theme="theme_name"

export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary $SECONDS $theme)"'

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
  
If you don't like the blank line spacer, you can remove the SECONDS from your .bashrc or .bash_profile file. It should work the same, just without the spacing.

SECONDS is only used for aesthetic reasons. I like the look of a blank line between each prompt. But, it has to be printed before the other printfs are called. If not, it will cause the input field to be below where intended. But printing it first leaves the issues of a blank line at the top upon opening, and the same when `clear` is used.

  In bash, SECONDS just adds 1 to itself every second the terminal is open. So we then know when the terminal has just opened, and we set up an alias to reset SECONDS when `clear` is called. Then we check if SECONDS is >1 to print the newline or not.

</details>

##

### Disclaimer
I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.


All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme

