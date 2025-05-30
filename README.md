# tokyobash
A custom bash prompt inspired by the [Tokyo Night Theme](https://www.github.com/folke/tokyonight.nvim) for [Neovim](https://www.github.com/neovim/neovim).
## Features
Will highlight the current directory and change color depending if you're in HOME/ root/ or /mnt.

![tokyobash_3](https://github.com/user-attachments/assets/ec448438-38d1-4ada-abb2-54e1fc064581)

And will abbreviate paths longer than 50 characters.

![tokyobash_4](https://github.com/user-attachments/assets/3529a96b-d37d-47a2-b383-906ad2801922)


## Installation
You will need a c compiler.
Either gcc or clang should be fine.

And add this to your .bashrc or .bash_profile:

```bash
SECONDS=0
export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary $SECONDS)"'
```
Then replace 'path/to/tokyobash/binary' above with the acutal path to the compiled tokyobash.c binary.

Save then restart your terminal!
## Disclaimer
I'm not sure if this will run on all linux distros. I'm also new to programming so any feedback is welcomed.

All screenshots were taken in ghostty terminal with tokyonight theme.
