#!/bin/bash
# -*- ENCODING: UTF-8 -*-
#--------------------3SO: Grupo A-P3--------------------------------------*/
#
# Autores:    Cesar Godino Rodriguez y Carmen Lopez Gonzalo               */
# Equipo:     catadioptric                                                */
#
#-------------------------------------------------------------------------*/

nombre_fich="test"

export curr_dir=$(pwd)

while [ "$nombre_fich" != "" ]
do
	echo "Nombre del fichero a simular (ENTER para terminar): "
	read nombre_fich

	if [ "$nombre_fich" == "" ]; then
		exit
	elif [ -f 'examples/'$nombre_fich ]; then

		num_cpus=0
		while [[ $num_cpus -le 0 || $num_cpus -gt 8 ]]
		do

			echo "Indica cuantas CPUs [1, 8] quieres usar en la simulacion: "
			read num_cpus

			if [[ $num_cpus -le 0 || $num_cpus -gt 8 ]]; then
				echo "Error al especificar cantidad de CPUs"
			else
				if [ -d resultados ]; then
					rm -r resultados
				fi
				mkdir resultados

				cant_algoritmos=4
				for ((i=0; i<$cant_algoritmos; i++))
				do
					algoritmo=""
					if [ $i -eq 0 ]; then
						algoritmo="RR"
					elif [ $i -eq 1 ]; then
						algoritmo="SJF"
					elif [ $i -eq 2 ]; then
						algoritmo="FCFS"
					elif [ $i -eq 3 ]; then
						algoritmo="PRIO"
					fi #schedsim, de no tener algoritmo un valor, se ejecutaria con -s sin especificar y daria error


					for ((j=0; j<$num_cpus; j++))
					do
						cant=$(($j+1))
						./schedsim -i 'examples/'$nombre_fich -s $algoritmo -n $cant
						cd ../gantt-gplot

						echo $curr_dir

						for ((k=0; k<=$j; k++))
						do
							./generate_gantt_chart $curr_dir/CPU_$k.log
						done


						cd $curr_dir/resultados
						mkdir ${algoritmo}_${cant}CPUS
						cd ..
						mv *.log resultados/${algoritmo}_${cant}CPUS
						mv *.eps resultados/${algoritmo}_${cant}CPUS

					done #for cpus por algoritmo


				done #for algoritmos

			fi

		done

	else
		echo "El fichero $nombre no existe"
	fi

done

exit
