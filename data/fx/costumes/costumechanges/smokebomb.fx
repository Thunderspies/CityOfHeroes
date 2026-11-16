#########################################################
##
##

FxInfo


Input
	InpName	Origin
End

Lifespan 100


Condition
	On Time
	Time 18
	Event
		Type	Local
		At	origin
		Sound gunslingershot3c 90 90 1.0
	End
End





Condition
	On 	Time
	Time 	19

	Event
		EName 	Flash3
		Type	Start
		At	Root
		Part	Costumes\CostumeChanges\SmallGlow.part	#middle glow
		Part	Costumes\CostumeChanges\LowerBlast.part
		Part	Costumes\CostumeChanges\LargeGlow.part 	#big glow
		part	Costumes\CostumeChanges\SmokePillar.part
		part	Costumes\CostumeChanges\SmokePillar2.part
		part	Costumes\CostumeChanges\SmokePillarAdditive.part
	End

End