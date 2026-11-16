#############################################################
## SmokeBomb.fx
#############################################################

FxInfo

Lifespan 100

Condition
	On	Time
	Time	10
	Event
		Type	Local
		At	origin
		Sound Ninja_SmokeBombEmote(noMX) 100 100 1
		# Sound	Ninja_SmokeBombEmote 100 100 1
	End
End

#############################################################

Condition
	On 	Time
	Time 	45

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