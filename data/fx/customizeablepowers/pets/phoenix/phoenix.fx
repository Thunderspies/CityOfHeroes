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
		Part1	:Bonfireflame_A2.part
		Part2	:Bonfireflame_B2.part
		Part4	:Bonfirespark.part
		Part5	:BonfireFlame_Flat.part
		Part5	:BonfireFlame_Flat_Small.part
		Sound flameburst_loop 70 70 .44
	End

	Event
		Type	local
		At	root
		Part2	:BonFireRingsFlat.part
		Part3	:BonFireRingsFlatThin.part
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
		Part1   :FlashFireRing.part
		Lifespan 30
	End
End

#############################################################

End