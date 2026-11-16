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
		SoundNoRepeat 3
		Sound 	Shivan_ChestBeam_Hit_01 35 35 .8
		Sound 	Shivan_ChestBeam_Hit_02 35 35 .8
		Sound 	Shivan_ChestBeam_Hit_03 35 35 .8
		Sound 	Shivan_ChestBeam_Hit_04 35 35 .8
		Sound 	Shivan_ChestBeam_Hit_05 35 35 .8
		Sound 	Shivan_ChestBeam_Hit_06 35 35 .8
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 -0.5 0.0
		End
		Part	:Hit_Middle.part
		Part	:Hit_Shockwave.part
		Lifespan	10
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 -0.5 0.0
		End
		Part	:Hit_Rays.part
		Lifespan	20
	End
End

#############################################################

End