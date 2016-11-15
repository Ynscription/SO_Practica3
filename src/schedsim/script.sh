#!/bin/bash
if [ -d resultados ]
then
	rm -rf resultados
fi

echo -n "Introduce el nombre del fichero a simular: "
read example
echo -n "Introduce el numero de cpus(min.1 / max. 8): "
read n_cpus

if [ "$n_cpus" -le 8 ] && [ "$n_cpus" -ge 1 ] && [ -f "examples/$example" ];
then

	mkdir resultados
	for nameSched in SJF RR PRIO FCFS
	do
		for j in `seq 1 $n_cpus`
		do
			cpu_dir=$j"_CPUS"

			if [ "$nameSched" == PRIO ] || [ "$nameSched" == SJF ]
			then
				echo "Simulating $nameSched-EXP with $j CPUS"
				mkdir -p resultados/$cpu_dir/$nameSched"-EXP"
				./schedsim -i examples/$example -n $j -s $nameSched -p
				i=0
				while [  $i -lt $j ]; do
	           	  	mv CPU_$i.log resultados/$cpu_dir/$nameSched"-EXP"/CPU-$i.log
					cd ../gantt-gplot
					./generate_gantt_chart ../schedsim/resultados/$cpu_dir/$nameSched"-EXP"/CPU-$i.log
					cd ../schedsim
	             	let i=i+1
	        	done
			fi

			echo "Simulating $nameSched with $j CPUS"
			./schedsim -i examples/$example -n $j -s $nameSched

			mkdir -p resultados/$cpu_dir/$nameSched
			i=0
        	while [  $i -lt $j ]; do

           	  	mv CPU_$i.log resultados/$cpu_dir/$nameSched/CPU-$i.log
				cd ../gantt-gplot
				./generate_gantt_chart ../schedsim/resultados/$cpu_dir/$nameSched/CPU-$i.log
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
