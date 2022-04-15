FROM wiiuenv/devkitppc:20220303

COPY --from=wiiuenv/wiiumodulesystem:20220204 /artifacts $DEVKITPRO
COPY --from=wiiuenv/libromfs_wiiu:20220305 /artifacts $DEVKITPRO
COPY --from=wiiuenv/libwuhbutils:20220415 /artifacts $DEVKITPRO

WORKDIR project