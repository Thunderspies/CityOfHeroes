#############################################################
## ChestBeam_Hit.fx
#############################################################

FxInfo
Lifespan 60

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	Shivan_Punch_Attack_01 200 200 .8
		Sound 	Shivan_Punch_Attack_02 200 200 .8
		Sound 	Shivan_Punch_Attack_03 200 200 .8
		Sound 	Shivan_Punch_Attack_04 200 200 .8
	End

	Event
		Type	Local
		At	Root
		SoundNoRepeat 4
		Sound 	Shivan_PunchVOX_01 30 30 .8
		Sound 	Shivan_PunchVOX_02 30 30 .8
		Sound 	Shivan_PunchVOX_03 30 30 .8
		Sound 	Shivan_PunchVOX_04 30 30 .8
		Sound 	Shivan_PunchVOX_05 30 30 .8
		Sound 	Shivan_PunchVOX_06 30 30 .8
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Hands_Rays.part
		Part	:Hands_Sparks.part
		Lifespan 45
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Hands_Rays.part
		Part	:Hands_Sparks.part
		Lifespan 45
	End

	Event
		Type	Local
		At	WepR
		Part	:Hands_Streaks.part
		Lifespan 23
	End

	Event
		Type	Local
		At	WepL
		Part	:Hands_Streaks.part
		Lifespan 23
	End
End

#############################################################

End