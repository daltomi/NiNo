#!/bin/bash

# Lugar en donde se encuentran los vídeos.
    videos="%s"

# Indica si se reproduce audio o no.
    audio=%s

# Separar la lista por salto de línea.
    SAVEIFS=$IFS
    IFS=$'\n'

# Listamos los archivos disponibles según formato.
    lista=$(ls $videos/{*.avi,*.mp4,*.flv,*.webm,*.ogv} 2>/dev/null)

# Esperar a que la ventana de xscreensaver esté lista.
    sleep 1s 

# Que bash nos envie algunas señales provenientes de xscreensaver.
    trap : SIGTERM SIGINT SIGHUP

# Iniciamos MPV en segundo plano.
    mpv --really-quiet $audio --fs --loop=inf --no-stop-screensaver --wid=$1 $lista &

# Restaurar
    IFS=$SAVEIFS

# El ID del proceso de MPV
    mpv_pid=$!

# Esperamos alguna señal de xscreensaver
    wait

# Le notificamos a MPV que estamos saliendo.
    kill $mpv_pid
