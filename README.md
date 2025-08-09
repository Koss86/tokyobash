# Tokyobash
A custom bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com) to use icons.
## Features

#### Highlights
Highlights the current directory and changes color depending on whether you're in HOME/, root/, or /mnt.
  
![tokyobash_01](https://github.com/user-attachments/assets/f8f1f02f-abf7-4a15-b024-491dcbb8675f)

#

#### Abbreviate Path
Abbreviates paths longer than 50 characters.

![tokyobash_02](https://github.com/user-attachments/assets/b3b94ac8-b229-442d-a684-4afb493649dc)

#

#### Git Branch Name
If current directory is a repository it will display the current branch name:
 
![tokyobash_03](https://github.com/user-attachments/assets/7a47c858-b828-4b0e-96d9-bff82d4d7d48)

#

#### Git Status Bar
An icon bar that tracks changes to current repository.

  ![tokyobash_statusbar](https://github.com/user-attachments/assets/a94962ee-7279-4cc3-acd5-1e97b9a74e4a)

Displays (in order):

  - Untracked files
  - Files with unstaged changes
  - Files with staged changes
  - Number of local commits
  - Number of remote commits.

![tokyobash_statusbarExp](https://github.com/user-attachments/assets/f3cce450-e44f-46aa-a4a2-b75f0145a9d5)

The status bar will only display when there are changes to the repo. And only show items that were changed.

It is disabled by default, but can be enabled via the `statusbar` [Config](#config) setting.

#

#### Themes
You can currently choose between 3 different themes.

Tokyonight:
  
![tokyobash_03](https://github.com/user-attachments/assets/75182622-9ce7-43a4-9fd0-d9f431105b26)

Catppuccin:

![tokyobash_catppuccin](https://github.com/user-attachments/assets/b99bbb0f-fe02-4c12-906b-51229efa3cb4)

Kanagawa:

![tokyobash_kanagawa](https://github.com/user-attachments/assets/3bc22047-44d2-4674-a20d-01537a94b5d7)

Can be changed via the `theme` [Config](#config) setting.

#

#### Theme Backgrounds
  
  Each theme has an optional background you can enable via the `background` [Config](#config) setting.
  
 Instead of changing path text color when in $HOME, mnt, or root it will change user background color.

  Tokyonight:
  
<img width="225" height="43" alt="tokyobash_tokyonight_bg" src="https://github.com/user-attachments/assets/9b1ff712-c651-42f1-9d1a-b2e7ef687648" />
<img width="225" height="43" alt="tokyobash_tokyonight_mnt_bg" src="https://github.com/user-attachments/assets/2a32ec22-434c-4782-ad5d-06998fa6e8b7" />
<img width="220" height="40" alt="tokyobash_tokyonight_root_bg" src="https://github.com/user-attachments/assets/378b3bc8-7e52-419f-9d0e-9a7dcdce8b58" />

#

Catppuccin:

<img width="225" height="43" alt="tokyobash_catppuccin_bg" src="https://github.com/user-attachments/assets/cfda0f0e-bd70-48b0-8f86-5a8575f5f3f8" />
<img width="225" height="43" alt="tokyobash_catppuccin_mnt_bg" src="https://github.com/user-attachments/assets/1a632eb0-7b47-47e1-b436-e5142c75806f" />
<img width="225" height="43" alt="tokyobash_catppuccin_root_bg" src="https://github.com/user-attachments/assets/099e43e2-4cc0-4604-924c-810fc12eae66" />

#

Kanagawa:

<img width="225" height="43" alt="tokyobash_kanagawa_bg" src="https://github.com/user-attachments/assets/a9f02c41-dce0-4e56-b8f0-66950f3a6a24" />
<img width="225" height="43" alt="tokyobash_kanagawa_mnt_bg" src="https://github.com/user-attachments/assets/a4a182d5-5e74-41a1-b2fc-bd65671db768" />
<img width="225" height="43" alt="tokyobash_kanagawa_root_bg" src="https://github.com/user-attachments/assets/8c986c88-1426-4ba5-947a-2b654660cb2c" />

## Installation

The easiest way would be to have make and gcc or clang installed on your system.

In the root of the tokyobash repository, there are 3 commands to use:


  -`make` Will build the tokyobash binary and place it in `repo_location/tokyobash/bin`.

  -`make install` Will build tokyobash and create the config file in `$XDG_CONFIG_HOME/tokyobash` if one is not already present:

  - If `$XDG_CONFIG_HOME` is not set, the config directory path will default to `$HOME/.config/tokyobash`

  -`make install prefix=custom/path` Will compile tokyobash, create and move config file, then move tokyobash to `prefix/bin`.
  
#

Now add this to your .bashrc (usually located at $HOME/.bashrc):

```bash

export PROMPT_COMMAND='PS1="$(path/to/tokyobash/binary)"'

# If tokyobash was placed somewhere that's in your $PATH,
# then your PROMPT_COMMAND can look like this.

export PROMPT_COMMAND='PS1="$(tokyobash)"'

```
Replace `path/to/tokyobash/binary` above with the acutal path to the compiled tokyobash binary.

Now save and restart your terminal!

#

#### No Make

If you dont have make, use these gcc commands in the top level of the tokyobash repository:
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

You will need to manually create the directory `tokyobash` in `$XDG_CONFIG_HOME`, if set. If you don't use `$XDG_CONFIG_HOME`, create the directory in `~/.config` and copy the config file there.

Try these commands in your terminal:

- If `XDG_CONFIG_HOME` not set:
  ```
  mkdir -p ~/.config/tokyobash/
  printf 'theme = tokyonight\n#theme = catppuccin\n#theme = kanagawa\n\ngit = 1\nbranchname = 1\nstatusbar = 0\ntime = 1\nfetchtimer = 1d' > ./config
  cp -n config ~/.config/tokyobash/
  rm config
  ```
- If  `XDG_CONFIG_HOME` is set:
  ```
  mkdir -p $XDG_CONFIG_HOME/tokyobash/
  printf 'theme = tokyonight\n#theme = catppuccin\n#theme = kanagawa\n\ngit = 1\nbranchname = 1\nstatusbar = 0\ntime = 1\nfetchtimer = 1d' > ./config
  cp -n config $XDG_CONFIG_HOME/tokyobash/
  rm config
  ```

## Config

Config file uses standard key = value pairs.

`#` or `//` can be used for comments.

Spaces, single, and double quotes are ignored.

`1 = enabled` `0 = disabled`

Default config:
```
theme       = tokyonight
#theme      = catppuccin
#theme      = kanagawa

background  = 0 # turn on/off background variant
git         = 1 # Turns all git integration on/off
branchname  = 1 # Turn the display of branch name on/off
statusbar   = 0 # Turn git status bar on/off
time        = 1 # Turn the display of current time on/off
fetchtimer  = 1d # See below.
```

`fetchtimer` will accept `#d`, `#h`, or `#m`. `d = days` `h = hours` and `m = minutes`.

For example:

  - If `fetchtimer = 25m` repo will update after 25 minutes.
  - If `fetchtimer = 2h` repo will update after 2 hours.
  - If `fetchtimer = 3d` repo will update after 3 days.
  - `fetchtimer = 1h30m` is not vaild and will probably cause an error. Only one `Day`, `Hour`, or `Minute` modifier allowed.

Setting `git = 0` disables all git integration. This supersedes the branchname and statusbar settings.
So, if `git = 0`, then branchname and statusbar are not displayed reguardless of their setting.

##

### Disclaimer
I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.
So if I'm doing somthing stupid (I'm sure I am somewhere) please let me know.

All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme
