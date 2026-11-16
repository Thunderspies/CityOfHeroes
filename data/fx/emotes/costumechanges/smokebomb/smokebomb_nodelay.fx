#############################################################
## SmokeBomb_NoDelay.fx
#############################################################

FxInfo
Lifespan 10

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash3
		Type	Start
		Sound Ninja_SmokeBombEmote(noMX) 100 100 1
		Lifespan 5
	End

	Event
		EName 	Flash3
		Type	Start
		At	FootL
		Part	:SmallGlow.part	#middle glow
		Part	:LowerBlast.part
		Part	:LargeGlow.part 	#big glow
		part	:SmokePillar.part
		part	:SmokePillar2.part
		part	:SmokePillarAdditive.part
		Lifespan 5
	End
End

#############################################################

End