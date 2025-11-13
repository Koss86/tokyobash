# Tokyobash

A configurable bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com)

## Features

### 🖌️ Highlighting

Highlights the current directory and changes the color of User@Host depending on whether you're in HOME/, root/, or /mnt.

#

### 🎨 Themes

There are currently three themes to choose from.

- Tokyonight:

  <img width="345" height="135" alt="Tokyobash_tokyonight_noBg" src="https://github.com/user-attachments/assets/5063d7aa-8a16-4c3c-b471-36833e9491d2" />

- Catppuccin:

  <img width="345" height="135" alt="Tokyobash_catppuccin_noBg" src="https://github.com/user-attachments/assets/e7cccf9d-343c-4630-bb74-5055240d1f8e" />

- Kanagawa:

  <img width="345" height="135" alt="Tokyobash_kanagawa_noBg" src="https://github.com/user-attachments/assets/fca86232-553e-47eb-a798-2b73f787c469" />

Themes can be changed via the `theme` [config](#config) setting.

#

#### 🌈 Backgrounds

There are optional backgrounds that can be enable via the `background` [Config](#config) setting.

There are three variatons to choose from:

<details>
  <summary>Pointed</summary>

- Tokyonight

  <img width="385" height="138" alt="Tokyobash_tokyonight_Bg_pointed" src="https://github.com/user-attachments/assets/1138846e-1c49-4925-9d96-46385dacaa8c" />

- Catppuccin

  <img width="385" height="138" alt="Tokyobash_catppuccin_Bg_pointed" src="https://github.com/user-attachments/assets/f72b5b94-7880-4830-ad4e-50b0c5457e40" />

- Kanagawa

  <img width="385" height="138" alt="Tokyobash_kanagawa_Bg_pointed" src="https://github.com/user-attachments/assets/6239f543-83e9-4926-84bc-7ce431979602" />

</details>

<details>
  <summary>Rounded</summary>

- Tokyonight

  <img width="375" height="138" alt="Tokyobash_tokyonight_Bg_rounded" src="https://github.com/user-attachments/assets/719405ca-4d1d-44da-b5aa-f857b983dc05" />

- Catppucin

  <img width="375" height="138" alt="Tokyobash_catppuccin_Bg_rounded" src="https://github.com/user-attachments/assets/d2818783-7762-4f4e-bee8-6ad59f61478c" />

- Kanagawa

  <img width="375" height="138" alt="Tokyobash_kanagawa_Bg_rounded" src="https://github.com/user-attachments/assets/f40ef138-3b0c-4c37-9b10-acad50cd0690" />

</details>

<details>
  <summary>Slanted</summary>

- Tokyonight

  <img width="395" height="138" alt="Tokyobash_tokyonight_Bg_slanted" src="https://github.com/user-attachments/assets/6fecc7bf-855e-4f5e-a30c-dc37c3276c96" />

- Catppuccin

  <img width="395" height="138" alt="Tokyobash_catppuccin_Bg_slanted" src="https://github.com/user-attachments/assets/6f2fb176-1f5f-42b1-9c9c-2e6f7fc03d7e" />

- Kanagawa

  <img width="395" height="138" alt="Tokyobash_kanagawa_Bg_slanted" src="https://github.com/user-attachments/assets/324c59d7-a46c-4131-bb2b-bb5607d14209" />

</details>

#

### 🤏 Abbreviate Path

Abbreviates paths longer than 50 characters.

<img width="777" height="462" alt="Tokyobash_abv" src="https://github.com/user-attachments/assets/a7d00006-64c8-4b2f-9d61-06441e7a8ddf" />

#

### 🌱 Branch Name

If the current directory is a repository it will display the current branch name.

<img width="497" height="45" alt="Tokyobash_branchname" src="https://github.com/user-attachments/assets/a334e841-27bf-4835-951b-f78388c5319c" />

#

### 🔍 Status Bar

An icon bar that displays changes in the current repository.

<img width="495" height="70" alt="Tokyobash_statusbar" src="https://github.com/user-attachments/assets/61efba08-11e5-4e10-8c2e-d17d848bbddf" />

Displays (in order):

- Number of untracked files
- Number of files with unstaged changes
- Number of staged files
- Number of local commits
- Number of remote commits.

<img width="497" height="169" alt="Tokyobash_statusbar_explained" src="https://github.com/user-attachments/assets/55761db9-6fa0-4451-9388-2ef01a0e4fef" />

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

##### Config File

You will need to manually create the directory `tokyobash` in `~/.config` or wherever you may have `$XDG_CONFIG_HOME` set.

Try these commands in your terminal. They will create the `tokyobash/config` directory and file in your `XDG_CONFIG_HOME` directory loaded with the defaults.

```sh
mkdir -p $XDG_CONFIG_HOME/tokyobash/
{
    printf 'theme      = tokyonight\n'
    printf '#theme     = catppuccin\n'
    printf '#theme     = kanagawa\n\n'
    printf 'background = 0\n'
    printf 'time       = 1\n'
    printf 'abvpath    = 1\n'
    printf 'branchname = 1\n'
    printf 'statusbar  = 1\n'
    printf 'fetch      = 0\n'
    printf 'fetchtimer = 1d\n'
    printf 'icon       = \n'
} >$XDG_CONFIG_HOME/tokyobash/config
```

#

### 🚧 Disclaimer

I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.
So if I'm doing somthing stupid (I'm sure I am somewhere) please let me know.

All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme.
