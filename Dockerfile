FROM ghcr.io/wiiu-env/devkitppc:20240423

COPY --from=ghcr.io/wiiu-env/wiiumodulesystem:20230719 /artifacts $DEVKITPRO
COPY --from=ghcr.io/wiiu-env/libromfs_wiiu:20230621 /artifacts $DEVKITPRO
COPY --from=ghcr.io/wiiu-env/libwuhbutils:20230621 /artifacts $DEVKITPRO

WORKDIR project
