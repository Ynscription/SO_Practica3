#!/bin/bash

rm -r resultados

echo -n "Introduce el nombre del fichero a simular: "
read example
echo -n "Introduce el numero de cpus(min.1 / max. 8): "
read n_cpus

if [ "$n_cpus" -le 8 ] && [ "$n_cpus" -ge 1 ] && [ -f "examples/$example" ]; 
then
	echo "Good"
	mkdir resultados
	for nameSched in SJF RR PRIO FCFS
	do
		for j in `seq 1 $n_cpus`
		do
			if [ "$nameSched" == PRIO ] || [ "$nameSched" == SJF ]
			then
				./schedsim -i examples/$example -n $n_cpus -s $nameSched -p
				i=0
				while [  $i -lt $n_cpus ]; do
	           	  	mv CPU_$i.log resultados/$nameSched-EXP-CPU-$i.log
					cd ../gantt-gplot				
					./generate_gantt_chart ../schedsim/resultados/$nameSched-EXP-CPU-$i.log
					cd ../schedsim	
	             	let i=i+1
	        	done
			fi 

			./schedsim -i examples/$example -n $n_cpus -s $nameSched

			i=0
        	while [  $i -lt $n_cpus ]; do
           	  	mv CPU_$i.log resultados/$nameSched-CPU-$i.log
				cd ../gantt-gplot				
				./generate_gantt_chart ../schedsim/resultados/$nameSched-CPU-$i.log
				cd ../schedsim	
             	let i=i+1
        	done
		done
	done

elif [ "$n_cpus" -le 0 ] || [ "$n_cpus" -gt 8 ]
then
	echo "El numero introducido de CPU's es incorrecto."
else 
	echo "El fichero con nombre $example no se ha encontrado."
fi
