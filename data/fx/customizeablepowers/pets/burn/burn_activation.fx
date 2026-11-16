#############################################################
## BurnPet.fx
#############################################################

FxInfo

#############################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	poisonGas1_loop 100 100 .4
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Root
		Part1	CustomizeablePowers\FireManipulation\Molotov_flame_A.part
		Part2	CustomizeablePowers\FireManipulation\Molotov_flame_B.part
		Part2	CustomizeablePowers\FireManipulation\Molotov_flame_C.part
		Part2	CustomizeablePowers\FireManipulation\MolotovBurnFlame01.part
		Part1	CustomizeablePowers\FireManipulation\MolotovBurnFlame02.part
		Part3	CustomizeablePowers\FireManipulation\Burnringspark.part
		Part4	CustomizeablePowers\FireManipulation\spark1.part
		Sound torch2_loop 40 40 .33
	End
End

## DECALS ###########################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	root
		Part	CustomizeablePowers\FireManipulation\HotFeetBaseFlat.part
		Part	CustomizeablePowers\FireManipulation\HotFeetBaseFlat2.part
	End
End

End