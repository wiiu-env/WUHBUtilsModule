[![CI-Release](https://github.com/wiiu-env/WUHBUtilsModule/actions/workflows/ci.yml/badge.svg)](https://github.com/wiiu-env/WUHBUtilsModule/actions/workflows/ci.yml)

## Usage
(`[ENVIRONMENT]` is a placeholder for the actual environment name.)

1. Copy the file `WUHBUtils.wms` into `sd:/wiiu/environments/[ENVIRONMENT]/modules`.  
2. Requires the [WUMSLoader](https://github.com/wiiu-env/WUMSLoader) in `sd:/wiiu/environments/[ENVIRONMENT]/modules/setup`.
3. Use [libwuhbutils](https://github.com/wiiu-env/libwuhbutils).

## Building using the Dockerfile

It's possible to use a docker image for building. This way you don't need anything installed on your host system.

```
# Build docker image (only needed once)
docker build . -t wuhbutils-builder

# make 
docker run -it --rm -v ${PWD}:/project wuhbutils-builder make

# make clean
docker run -it --rm -v ${PWD}:/project wuhbutils-builder make clean
```

## Format the code via docker

`docker run --rm -v ${PWD}:/src wiiuenv/clang-format:13.0.0-2 -r ./src -i`