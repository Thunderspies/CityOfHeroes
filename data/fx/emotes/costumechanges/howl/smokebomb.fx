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
	Time 31
	Event
		Type	Local
		At	origin
		Sound gunslingershot3c 90 90 1.0
	End
End





Condition
	On 	Time
	Time 	32

	Event
		EName 	Flash3
		Type	Start
		At	Root
		Part	Emotes\CostumeChanges\Lightning\SmallGlow.part	#middle glow
		Part	Emotes\CostumeChanges\Lightning\LowerBlast.part
		Part	Emotes\CostumeChanges\Lightning\LargeGlow.part 	#big glow
		part	:SmokePillar.part
		part	:SmokePillar2.part
		part	:SmokePillarAdditive.part
	End

End