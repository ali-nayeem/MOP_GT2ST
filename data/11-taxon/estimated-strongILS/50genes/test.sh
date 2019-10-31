#!/bin/bash
for i in {1..10}
do
	value=$(<R$i/species.tre_astral)"\n"$(<R$i/species.tre_mpest)"\n"$(<R$i/species.tre_stelar)"\n"$(<R$i/true_st.tre)
	echo -e "$value" > tool_tree/R$i.txt
done
