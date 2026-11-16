#############################################################
## Bonfire.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	start
		At	root
		Part1	WORLD\Ruins\Bonfireflame_A2.part
		Part2	WORLD\Ruins\Bonfireflame_B2.part
		Part4	WORLD\Ruins\Bonfirespark.part
		Part5	WORLD\Ruins\BonfireFlame_Flat.part
		Part5	WORLD\Ruins\BonfireFlame_Flat_Small.part
		Sound flameburst_loop 70 70 .44
	End

	Event
		Type	local
		At	root
		Part2	POWERS\FireControl\BonFireRingsFlat.part
		Part3	POWERS\FireControl\BonFireRingsFlatThin.part
		Sound torch2_loop 80 80 .33
	End
End

#############################################################

Condition
	On 	cycle
	Time 	60
	Chance	0.8

	Event
		EName 	Prime8
		Type	start
		At	Root
		Part1   POWERS\FireControl\FlashFireRing.part
		Lifespan 30
	End
End

#############################################################

End