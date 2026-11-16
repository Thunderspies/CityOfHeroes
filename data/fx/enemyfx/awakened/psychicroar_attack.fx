#############################################################
## PsychicRoar_Attack.fx
#############################################################

FxInfo
Lifespan 200

#############################################################
Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	Awakened_PsychicRoar_Attack_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	StartPositOnly
		At	Head
		part	:PsychicRoar_Rings.part
		Part	:PsychicRoar_Rings_Highlight.part
		Part	:PsychicRoar_Streaks_Central.part
		Lifespan 25
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:PsychicRoar_Light_Central.part
		Part	:PsychicRoar_Shockwave.part
		Part	:PsychicRoar_Streaks.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	10


	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors/Dust_Expanding.bhvr
		Part	:Dust_Expanding.part
		Lifespan 20
	End
End

#############################################################

End