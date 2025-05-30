# tokyobash
A custom bash prompt inspired by the [Tokyo Night Theme](https://www.github.com/folke/tokyonight.nvim) for [Neovim](https://www.github.com/neovim/neovim).
## Features
Will highlight the current directory and change color depending if you're in HOME/ root/ or /mnt.

![tokyobash_3](https://github.com/user-attachments/assets/a55c64af-8c00-4d4e-916e-570426bfab48)

And will abbreviate paths longer than 50 characters.

![tokyobash_4](https://github.com/user-attachments/assets/07c0e014-5961-48aa-87d4-71aa70f0a465)


## Installation
You will need a c compiler.
Either gcc or clang should be fine.

And add this to your .bashrc:

```bash
SECONDS=0
export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary $SECONDS)"'
```
Then replace 'path/to/tokyobash/binary' above with the acutal path to the compiled tokyobash.c binary.

Save then restart your terminal!
## Disclaimer
I'm not sure if this will run on all linux distros. I'm also new to programming so any feedback is welcomed.

All screenshots were taken in ghostty terminal with tokyonight theme.
