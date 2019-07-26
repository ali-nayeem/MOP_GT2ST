 #!/usr/bin/bash
for i in {01..10}
do
        rm truegenetrees*
        cp "true-gene-trees/"$i truegenetrees1
        ./mpest  ./truegenetrees1  ./specieslist15 1
        cp truegenetrees1.best.of.1.tre "true-gene-trees/"$i".mpest"
        rm control*
done
