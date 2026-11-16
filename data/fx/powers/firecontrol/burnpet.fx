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
		At	root
		Part1	WeaponFx\Molotov_flame_A.part
		Part2	WeaponFx\Molotov_flame_B.part
		Part4	WeaponFx\spark1.part
		Part2	WeaponFx\Molotov_flame_C.part
	End

	Event
		Type	start
		At	Root
		Part2	powers\firecontrol\MolotovBurnFlame01.part
		Part3	powers\firecontrol\Burnringspark.part
		Sound torch2_loop 40 40 .33
	End

	Event
		Type	start
		At	Root
		Part1	powers\firecontrol\MolotovBurnFlame02.part
	End
End

## SPLATS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Root
		Part	:HotFeetBaseFlat.part
		Part	:HotFeetBaseFlat2.part
		Lifespan 20
	End
End

#############################################################

End