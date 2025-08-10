# Tokyobash
A custom bash prompt inspired by the [Tokyo Night](https://www.github.com/folke/tokyonight.nvim) Theme for [Neovim](http://www.neovim.io).

Requires a [Nerd Font](https://www.nerdfonts.com) to use icons.
## Features

### Highlights
Highlights the current directory and changes color depending on whether you're in HOME/, root/, or /mnt.
  
![tokyobash_01](https://github.com/user-attachments/assets/f8f1f02f-abf7-4a15-b024-491dcbb8675f)

#

### Abbreviate Path
Abbreviates paths longer than 50 characters.

<img width="750" height="1000" alt="tokyobash_02" src="https://github.com/user-attachments/assets/b3b94ac8-b229-442d-a684-4afb493649dc" /><br>

#

### Git Branch Name
If the current directory is a repository it will display the current branch name:
 
![tokyobash_03](https://github.com/user-attachments/assets/7a47c858-b828-4b0e-96d9-bff82d4d7d48)

#

### Git Status Bar
An icon bar that tracks changes to the current repository.

  ![tokyobash_statusbar](https://github.com/user-attachments/assets/a94962ee-7279-4cc3-acd5-1e97b9a74e4a)

Displays (in order):

  - Untracked files
  - Files with unstaged changes
  - Files with staged changes
  - Number of local commits
  - Number of remote commits.

![tokyobash_statusbarExp](https://github.com/user-attachments/assets/f3cce450-e44f-46aa-a4a2-b75f0145a9d5)

The status bar will only display when there are changes to the repo. And only show items that were changed.
> [!NOTE]
>
> The feature to fetch remote commits is disabled by default.
>
> But it can be enabled via the `fetch` [Config](#config) setting.

#

### Themes
You can currently choose between 3 different themes.

#### Tokyonight:
  
![tokyobash_03](https://github.com/user-attachments/assets/75182622-9ce7-43a4-9fd0-d9f431105b26)

#### Catppuccin:

![tokyobash_catppuccin](https://github.com/user-attachments/assets/b99bbb0f-fe02-4c12-906b-51229efa3cb4)

#### Kanagawa:

![tokyobash_kanagawa](https://github.com/user-attachments/assets/3bc22047-44d2-4674-a20d-01537a94b5d7)

Can be changed via the `theme` [Config](#config) setting.

#

### Theme Background Variants
  
  Each theme has an optional background variant you can enable via the `background` [Config](#config) setting.
  
 Instead of changing the path text color when in $HOME, mnt, or root it will change the user background color.

  #### Tokyonight:
  
<img width="600" height="60" alt="tokyobash_tokyonight_bg" src="https://github.com/user-attachments/assets/226a16e1-78df-49ec-825d-3f0c1b4e86db" /><br>
<img width="370" height="60" alt="tokyobash_tokyonight_bg_mnt" src="https://github.com/user-attachments/assets/1b693799-e1fc-4690-a869-c45952ea9d69" /><br>
<img width="325" height="60" alt="tokyobash_tokyonight_bg_root" src="https://github.com/user-attachments/assets/dd928ffc-1b9e-432f-81c6-ba327abc580f" /><br>

#

#### Catppuccin:

<img width="600" height="60" alt="tokyobash_catppuccin_bg" src="https://github.com/user-attachments/assets/dd220cf3-9e6a-4eea-8aff-98214fb6df2b" /><br>
<img width="370" height="60" alt="tokyobash_catppuccin_bg_mnt" src="https://github.com/user-attachments/assets/8ca8a448-5eb6-423f-b6fb-42b6e10f5fbd" /><br>
<img width="325" height="60" alt="tokyobash_catppuccin_bg_root" src="https://github.com/user-attachments/assets/475552cf-6c46-497f-8870-002a5fbeb992" /><br>

#

#### Kanagawa:

<img width="600" height="60" alt="tokyobash_kanagawa_bg" src="https://github.com/user-attachments/assets/16d85aec-f773-49f1-8b55-2fb405fc688c" /><br>
<img width="368" height="60" alt="tokyobash_kanagawa_bg_mnt" src="https://github.com/user-attachments/assets/5fe83ea8-c51a-40f8-a083-292ff48f7003" /><br>
<img width="325" height="60" alt="tokyobash_kanagawa_bg_root" src="https://github.com/user-attachments/assets/7c70331e-2013-4439-9e3e-244dfedba1e0" /><br>


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
gcc -c src/lib/gitlib.c
gcc -c src/lib/shouldFetchlib.c
gcc -c src/lib/printlib.c
gcc -O3 tokyobash.o tokyobashlib.o gitlib.o shouldFetchlib.o printlib.o -o tokyobash
```

For clang:

```
clang -c src/tokyobash.c
clang -c src/lib/tokyobashlib.c
clang -c src/lib/gitlib.c
clang -c src/lib/shouldFetchlib.c
clang -c src/lib/printlib.c
clang -O3 tokyobash.o tokyobashlib.o gitlib.o shouldFetchlib.o printlib.o -o tokyobash
```

You will need to manually create the directory `tokyobash` in `$XDG_CONFIG_HOME`, if set. If you don't use `$XDG_CONFIG_HOME`, create the directory in `~/.config` and copy the config file there.

Try these commands in your terminal:

- If `XDG_CONFIG_HOME` not set:
  ```
  mkdir -p ~/.config/tokyobash/
  printf 'theme = tokyonight\n#theme = catppuccin\n#theme = kanagawa\n\nbackground = 0\ngit = 1\ntime = 1\nbranchname = 1\nstatusbar = 0\nfetch = 0\nfetchtimer = 1d' > ./config
  cp -n config ~/.config/tokyobash/
  rm config
  ```
- If  `XDG_CONFIG_HOME` is set:
  ```
  mkdir -p $XDG_CONFIG_HOME/tokyobash/
  printf 'theme = tokyonight\n#theme = catppuccin\n#theme = kanagawa\n\nbackground = 0\ngit = 1\ntime = 1\nbranchname = 1\nstatusbar = 0\nfetch = 0\nfetchtimer = 1d' > ./config
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

background  = 0  # Turn background variant on/off.
git         = 1  # Turns all git integration on/off.
time        = 1  # Turn the display of current time on/off.
branchname  = 1  # Turn the display of branch name on/off.
statusbar   = 1  # Turn git status bar on/off.
fetch       = 0  # Turn fetching of commits on/off.
fetchtimer  = 1d # See below.
```

`fetchtimer` will accept `#d`, `#h`, or `#m`. `d = days` `h = hours` and `m = minutes`.

For example:

  - If `fetchtimer = 25m` repo will update after 25 minutes.
  - If `fetchtimer = 2h` repo will update after 2 hours.
  - If `fetchtimer = 3d` repo will update after 3 days.
  - `fetchtimer = 1h30m` is not vaild. Only one `Day`, `Hour`, or `Minute` modifier is allowed.

Setting `git = 0` disables all git integration. This supersedes the branchname and statusbar settings.
So, if `git = 0`, then branchname and statusbar are not displayed reguardless of their setting.

##

### Disclaimer
I'm not sure if this will run on all linux distros. I believe it should work on most if not all.

I'm also new to programming so any feedback is welcomed.
So if I'm doing somthing stupid (I'm sure I am somewhere) please let me know.

All screenshots were taken with: [ghostty](https://www.ghostty.org) terminal with tokyonight theme
