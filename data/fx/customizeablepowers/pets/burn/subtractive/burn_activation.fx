#############################################################
## BurnPet.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Root
		Part1	CustomizeablePowers\FireManipulation\Subtractive\Molotov_flame_A.part
		Part2	CustomizeablePowers\FireManipulation\Subtractive\Molotov_flame_B.part
		Part2	CustomizeablePowers\FireManipulation\Subtractive\Molotov_flame_C.part
		Part2	CustomizeablePowers\FireManipulation\Subtractive\MolotovBurnFlame01.part
		Part1	CustomizeablePowers\FireManipulation\Subtractive\MolotovBurnFlame02.part
		Part3	CustomizeablePowers\FireManipulation\Subtractive\Burnringspark.part
		Part4	CustomizeablePowers\FireManipulation\Subtractive\spark1.part
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
		Part	CustomizeablePowers\FireManipulation\Subtractive\HotFeetBaseFlat.part
		Part	CustomizeablePowers\FireManipulation\Subtractive\HotFeetBaseFlat2.part
	End
End

End