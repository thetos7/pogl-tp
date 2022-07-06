# POGL Snow

## Running

on the project root, run
```sh
make release
```

then go into the generated build directory (`build` by default)
and run (`-j` option recommended to save time)
```sh
make
```
to compile.

finally, to launch the compiled application, run it with
```sh
./opengl
```

## Debugging

Same procedure as for running the release version, replacing the root make target with `debug`

## Creating an exportable tar file

When at the root of the project, run:
```sh
make archive
```
