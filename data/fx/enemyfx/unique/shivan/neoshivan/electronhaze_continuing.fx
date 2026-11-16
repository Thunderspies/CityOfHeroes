#############################################################
## ElectronHaze.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	ShivanNeo_ElectronHaze_Hit_01 100 100 .8
		Sound 	ShivanNeo_ElectronHaze_Hit_02 100 100 .8
		Sound 	ShivanNeo_ElectronHaze_Hit_03 100 100 .8
		Sound 	ShivanNeo_ElectronHaze_Hit_04 100 100 .8
		Sound 	ShivanNeo_ElectronHaze_Hit_05 100 100 .8
		Sound 	ShivanNeo_ElectronHaze_Hit_06 100 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	HandAnchorR
		Type	PositOnly
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:ElectronHaze_Rays_Chest.part
		part	:Electron_Trail_Stationary_Chest.part

		Lifespan 90
	End

End

#############################################################

End