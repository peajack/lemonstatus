[screenshot demonstrating how does it look like](./screenshot.png)

# lemonstatus

generate status for lemonbar. use it with openbsd (kqueue rules!) and cwm.

# build

you need scdoc(1) for manpage

edit config.mk for your needs

```
make install
```

# usage

```sh
lemonstatus | lemonbar -pd -B '#282828' -F '#ebdbb2' 
```

see lemonstatus(1)
