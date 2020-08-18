#!/bin/bash

################################################################################

# Tipos de respuestas para todos los protocolos.
    REPLY_YES=%s  # Respuesta Si.
    REPLY_NO=%s   # Respuesta No.

# Protocolos de Error. 
# Espera una respuesta de parte de NiNo, Sí o No.
    ERR_Q_PROTO=%s # Error de pregunta.
    ERR_I_PROTO=%s # Error de información.
    ERR_W_PROTO=%s # Error de alerta.

# Protocolos de Informaciones.
# Espera una respuesta de parte de NiNo, Sí solamente.
    INFO_PROTO=%s  # Información.
    WARN_PROTO=%s  # Alerta.

# Protocolos de Salida. Silenciosa.
# No espera ninguna respuesta de parte de NiNo
    QUIT_PROTO=%s  # Salir de NiNo

# Delimitador de formato   
    DELIM=%s

################################################################################

# Archivo de configuración de xscreensaver
    archivo_xscr=$HOME/.xscreensaver

# Nombre y ubicación del script seleccionado.
    temporal=%s

# Titulo del programa mostrado por xscreensaver-demo
    titulo="%s" 

# Archivo FIFO
    fifo=%s    

# Ubicación de destino del script
    destino=$HOME/.local/bin

# Quitar los 6 dígitos aleatorios del temporal y se añade la extension.
     script=$(echo $temporal | head -c -7).sh

# Quitar la ubicación temporal
     script=${script##*/}

################################################################################

# Retorna 0 para no instalar o 1 para instalar.
    function antes_de_instalar () {
        local titulo="NiNo - Error"
        local usr_bin="/usr/bin"
        local usr_local_bin="/usr/local/bin"

        # Verificar si existe el programa MPV
        if [ ! -e $usr_bin/mpv ] && [ ! -e $usr_local_bin/mpv ]; then
            local mensaje="No se encontró instalado el programa MPV."
            echo $WARN_PROTO$DELIM$titulo$DELIM$mensaje > $fifo
            sync
            read -n 1 continuar < $fifo
            return 0
        fi
        
        # Verificar si existe el programa xscreensaver-demo
        if [ ! -e $usr_bin/xscreensaver-demo ] && [ ! -e $usr_local_bin/xscreensaver-demo ]; then
            local mensaje="No se encontró instalado el programa xscreensaver-demo."
            echo $WARN_PROTO$DELIM$titulo$DELIM$mensaje > $fifo
            sync
            read -n 1 continuar < $fifo
            return 0
        fi

        # Verificar si existe el archivo .xscreensaver
        if [ ! -e "$archivo_xscr" ]; then
            local mensaje="El archivo $archivo_xscr no existe. Ejecute xscreensaver-demo primero."
            echo $WARN_PROTO$DELIM$titulo$DELIM$mensaje > $fifo
            sync
            read -n 1 continuar < $fifo
            return 0
        fi

        # Verificar que no exista ya el script...
        local archivo=$destino/$script

        if [ -e "$archivo" ]; then
            titulo="NiNo - Pregunta"
            local mensaje="El archivo $archivo ya existe. ¿Sobrescribirlo?"         
            echo $ERR_Q_PROTO$DELIM$titulo$DELIM$mensaje > $fifo
            sync 
            read -n 1 continuar < $fifo

            if [ "$continuar" == "$REPLY_NO" ]; then return 0; fi
        fi

        return 1
    }


    function actualizar_xscr () {
        # Busca la última linea de 'programs':
            linea=$(grep -n  '\\n\\' $archivo_xscr | cut -d: -f1 | tail -n 1)

        # Escribir en la siguiente.
            linea=$(($linea + 1))

        # Ítem para 'programs:'
            programa="\"$titulo\" $destino/$script \$XSCREENSAVER_WINDOW \\n\\"

        # Agregamos el ítem en dónde corresponde.
            printf '%s\n' H ${linea}i "$programa" . wq | ed -s $archivo_xscr
    }


    function instalar () {
        # Crear el directorio por si no existe.   
            mkdir -p $destino

        # .. se continua, copiar el script al directorio bin.
            cp $temporal $destino/$script

            chmod +x $destino/$script
    
        # Actualizar .xscreensaver
            actualizar_xscr
        
            local titulo="Nino - Instalación"
            local informacion="La instalación ha terminado con éxito."

            echo $INFO_PROTO$DELIM$titulo$DELIM$informacion > $fifo
            sync

            read -n 1 continuar < $fifo

        # Envia señal de sí 'Salir de NiNo'
            echo $QUIT_PROTO$DELIM$REPLY_YES > $fifo
            sync
    }


# PROGRAMA
################################################################################
    antes_de_instalar
    
    if [ "$?" == "1" ]; then instalar; fi

    exit 0
################################################################################