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
		Type	StartPositOnly
		At	Chest
		BhvrOverride
			PositionOffset	0 -0.5 0.0
		End
		Part	:Hit_Middle_Punch.part
		Part	:Hit_Shockwave_Punch.part
		part	:Hit_Sparks.part
		part	:Hit_Rocks.part
		Part	:Hit_Rays_Punch.part
		Part	:Hit_Smoke.part
		Part	:Hit_Streaks.part
		Lifespan	2
	End

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	Shivan_Punch_Hit_01 200 200 .8
		Sound 	Shivan_Punch_Hit_02 200 200 .8
		Sound 	Shivan_Punch_Hit_03 200 200 .8
		Sound 	Shivan_Punch_Hit_04 200 200 .8
	End

End

#############################################################

End