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
		Part	:Hit_Middle_Punch.part
		Part	:Hit_Flash.part

		Part	:Hit_Shockwave_Punch.part
		Part	:Hit_Streaks.part
		Lifespan	2
	End

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	ShivanNeo_X-RayBeam_Hit_01 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Hit_02 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Hit_03 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Hit_04 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Hit_05 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Hit_06 200 200 .8
	End

End

#############################################################

End