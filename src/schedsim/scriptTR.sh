#!/bin/bash
if [ -d resultados ]
then
	rm -rf resultados
fi


mkdir resultados
for nameSched in PRIO PRIOTR
do
	echo "Simulating $nameSched with 1 CPUS"
	./schedsim -i examples/exampleA1.txt -s $nameSched -p

  	mv CPU_0.log resultados/$nameSched"_CPU-0.log"
	cd ../gantt-gplot
	./generate_gantt_chart ../schedsim/resultados/$nameSched"_CPU-0.log"
	cd ../schedsim
done
