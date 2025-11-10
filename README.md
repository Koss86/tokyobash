# Tokyobash

A configurable bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com)

## Features

### 🖌️ Highlighting

Highlights the current directory and changes the color of User@Host depending on whether you're in HOME/, root/, or /mnt.

<img width="484" height="163" alt="tok_noBg_spread" src="https://github.com/user-attachments/assets/c5efd9d7-79af-48c2-9617-d478631ee710" />

#

### 🎨 Themes

There are currently three themes to choose from.

- Tokyonight:

  <img width="484" height="163" alt="tok_noBg_spread" src="https://github.com/user-attachments/assets/b97f6193-910f-4814-bcd5-e7d1ef05fc1c" />

- Catppuccin:

  <img width="485" height="164" alt="cat_noBg_spread" src="https://github.com/user-attachments/assets/b0428e1a-1eab-4153-a92f-eefa85b9daeb" />

- Kanagawa:

  <img width="487" height="163" alt="kan_noBg_spread" src="https://github.com/user-attachments/assets/dacd4d04-8dbc-4e26-bf34-f6851e75e4e4" />

Themes can be changed via the `theme` [config](#config) setting.

#

#### 🌈 Backgrounds

There are optional backgrounds that can be enable via the `background` [Config](#config) setting.

There are three variatons to choose from: Pointed, Rounded, and Slanted. Pointed is shown below.

- Tokyonight:

  <img width="544" height="163" alt="tok_Bg_spread" src="https://github.com/user-attachments/assets/fb3a5426-da87-4718-bb3e-5f623ebdd757" />

- Catppuccin:

  <img width="545" height="163" alt="cat_Bg_spread" src="https://github.com/user-attachments/assets/785798e4-41eb-4418-97ea-27de9bab1b60" />

- Kanagawa:

  <img width="546" height="163" alt="kan_Bg_spread" src="https://github.com/user-attachments/assets/a45d4003-cbb4-45f7-9bc9-ae684b6c1cbf" />

#

### 🤏 Abbreviate Path

Abbreviates paths longer than 50 characters.

<img width="788" height="366" alt="tok_noBg_abv" src="https://github.com/user-attachments/assets/b8dd67cf-cb70-4d86-a2d6-cab5095f67b9" />

#

### 🌱 Branch Name

If the current directory is a repository it will display the current branch name.

<img width="485" height="46" alt="tok_branchName" src="https://github.com/user-attachments/assets/443d36ad-83c0-4d88-a008-dad27f7dc332" />

#

### 🔍 Status Bar

An icon bar that displays changes in the current repository.

<img width="487" height="65" alt="statusbar_clean" src="https://github.com/user-attachments/assets/b3596724-b5d4-4689-aad7-8a27bfdd711a" />

Displays (in order):

- Number of untracked files
- Number of files with unstaged changes
- Number of staged files
- Number of local commits
- Number of remote commits.

<img width="487" height="126" alt="tokyobash_statusbar_explained" src="https://github.com/user-attachments/assets/ef044331-ef41-4f80-a320-caa8c8689eb5" />

> [!NOTE]
>
> The feature to auto-fetch commits is disabled by default.
>
> Can be enabled via the `fetch` [Config](#config) setting.
>
> And modified with the `fetchtimer` [Config](#config) setting.

## 🔧 Installation

The easiest way would be to have make and gcc/clang installed on your system.
For instructions on how to install without make click [here](#no-make).

In the root of the tokyobash repository, there are three commands to use:

- `make` Will build the tokyobash binary and place it in `/bin` in the root of the tokyobash repository.

* `make install` Will build tokyobash, create and then move the config file to `$XDG_CONFIG_HOME/tokyobash` if one is not already present.
  - The config directory defaults to `$XDG_CONFIG_HOME`. If for any reason `$XDG_CONFIG_HOME` can't be resolved, the config directory will default to `$HOME/.config`.

- `make install prefix=custom/path` Will build tokyobash, create and move config file, then move tokyobash to `prefix/bin`.

#

Add this to your .bashrc (usually located at $HOME/.bashrc):

```bash
export PROMPT_COMMAND='PS1="$(path/to/tokyobash)"'
```

Replace `path/to/tokyobash` above with the acutal path to the compiled tokyobash binary.

If tokyobash was placed somewhere that's in your `$PATH`, then your `PROMPT_COMMAND` can look like this.

```bash
export PROMPT_COMMAND='PS1="$(tokyobash)"'
```

Now save and restart your terminal!

## ⚙️ Config

Config file uses standard key = value pairs.

`#` or `//` can be used for comments.

Spaces, single, and double quotes are ignored.

`1 = enabled` `0 = disabled` - except for `background`, `time`, and `fetchtimer`, which have multiple options explained below.

#### Default config:

```
theme       = tokyonight
#theme      = catppuccin
#theme      = kanagawa

background  = 0  # 0 = Off, 1 = pointed, 2 = rounded, 3 = slanted.
time        = 1  # 0 = Off, 1 = 24-hour clock, 2 = 12-hour clock.
abvpath     = 1  # Abberviate path on/off
branchname  = 1  # Turn the display of branch name on/off.
statusbar   = 1  # Turn statusbar on/off.
fetch       = 0  # Turn fetching of commits on/off.
fetchtimer  = 1d # See below.
icon        =   # Change icon or set to 0 to remove.
```

`fetchtimer` will accept `#d`, `#h`, or `#m`. `d = days` `h = hours` and `m = minutes`.

##### For example:

- If `fetchtimer = 25m` repository will update after 25 minutes, when the prompt reloads.
- If `fetchtimer = 2h` repository will update after 2 hours, when the prompt reloads.
- If `fetchtimer = 3d` repository will update after 3 days, when the prompt reloads.
- `fetchtimer = 1h30m` is not vaild. Only one `Day`, `Hour`, or `Minute` modifier is allowed.

## 🚫 No Make

All you really need is a C compiler to build Tokyobash.

So, if you dont have make you can use these commands in the top level of the tokyobash repository.

For gcc:

```sh
gcc -c src/tokyobash.c
gcc -c src/lib/tokyobashlib.c
gcc -c src/lib/gitlib.c
gcc -c src/lib/shouldFetchlib.c
gcc -c src/lib/printlib.c
gcc -O3 tokyobash.o tokyobashlib.o gitlib.o shouldFetchlib.o printlib.o -o tokyobash
rm tokyobash.o tokyobashlib.o gitlib.o shouldFetchlib.o printlib.o
```

For clang:

```sh
clang -c src/tokyobash.c
clang -c src/lib/tokyobashlib.c
clang -c src/lib/gitlib.c
clang -c src/lib/shouldFetchlib.c
clang -c src/lib/printlib.c
clang -O3 tokyobash.o tokyobashlib.o gitlib.o shouldFetchlib.o printlib.o -o tokyobash
rm tokyobash.o tokyobashlib.o gitlib.o shouldFetchlib.o printlib.o
```

You don't need a config file if you don't plan on changing the defaults. But, if you do want to change the defaults
you will need to manually create the directory `tokyobash` in `~/.config` or wherever you may have `$XDG_CONFIG_HOME` set.

Try these commands in your terminal:

```sh
mkdir -p $XDG_CONFIG_HOME/tokyobash/
printf 'theme = tokyonight\n# theme = catppuccin\n# theme = kanagawa\n\nbackground = 0\ntime = 1\nabvpath = 1\nbranchname = 1\nstatusbar = 1\nfetch = 0\nfetchtimer = 1d' > ./config
cp -n config $XDG_CONFIG_HOME/tokyobash/
rm config
```

#

### 🚧 Disclaimer

I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.
So if I'm doing somthing stupid (I'm sure I am somewhere) please let me know.

All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme.
