# Tokyobash
A custom bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).
## Features
Will highlight the current directory and change color depending if you're in HOME/ root/ or /mnt.

![tokyobash_1](https://github.com/user-attachments/assets/401314ad-97fa-4125-8ec3-8c07f5b321dc)

And will abbreviate paths longer than 50 characters.

![tokyobash_2](https://github.com/user-attachments/assets/380b6184-6d09-4616-8278-6d7c01b70cc0)


## Installation
You will need a c compiler. Either gcc or clang should be fine.

Navigate to your tokyobash.c file location in your terminal and type:
```
gcc -o tokyobash tokyobash.c
```
Or if you're using clang, just replace gcc with clang and it will work the same.
This will create the binary you will need the path for below.

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
Then replace 'path/to/tokyobash/binary' above with the acutal path to the compiled tokyobash.c binary.

Save then restart your terminal!
<details>
<summary>What's with SECONDS?</summary>
  
SECONDS is only used for aesthetic reasons. I like the look of a blank line between each prompt. But, it has to be printed before the other printfs are called. If not, it will cause the input field to be below where intended. But printing it first leaves the issues of a blank line at the top upon opening, and the same when `clear` is used.

  In bash, SECONDS just adds 1 to itself every second the terminal is open. So we know when the terminal just opened and we set up and alias to reset SECONDS when `clear` is called. Then in our code we have a check if SECONDS is >1 to print the newline or not.

  So if you're not a fan of the blank line spacer, you can just remove the check for argv and references to SECONDS in your bash file and it will work just the same just without the spacing.
</details>

### Known Issues
If another program, like git, alters the font color it will be reset back to default until the next prompt prints.

![tokyobash_iss1](https://github.com/user-attachments/assets/b1336943-52a6-4d3c-a815-cedd1f83ad13)

### Disclaimer
I'm not sure if this will run on all linux distros. I'm also new to programming so any feedback is welcomed.

All screenshots were taken with:

[ghostty](https://www.ghostty.org) terminal with tokyonight theme

SourceCodePro [Nerd Font](https://www.nerdfonts.com).
