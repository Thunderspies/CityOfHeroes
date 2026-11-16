#############################################################
## Knockout_FadeToBlack.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

#	Event
#		EName	Effect01
#		Type	Local
#		At	Root
#		Sound 	FirstWard_ShadowyPathsEnter_01 100 100 .65
#	End
#
#	Event
#		EName	Effect02
#		Type	Local
#		At	Root
#		Sound 	FirstWard_ShadowyComets_Loop 100 100 .35
#		bhvr	behaviors/Sound/SoundIn3Out5.bhvr
#	End
#
#	Event
#		EName	Effect03
#		Type	Local
#		At	Root
#		Sound FirstWard_ShadowyPaths_Loop 100 100 0.6
#		bhvr	behaviors/Sound/SoundIn3Out5.bhvr
#	End

	Event
		EName	Effect04
		Type	Start
		CameraSpace
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:NW_Redout_Edges.part
		Part	:NW_Screen_Particles.part
	End



End

Condition
	On 	Death

	Event
		EName	Effect02
		Type 	Destroy
	End

	Event
		EName	Effect03
		Type 	Destroy
	End

	Event
		EName	Effect04
		Type 	Destroy
	End

	Event
		Type	Local
		At	Root
		Sound 	FirstWard_ShadowyPathsExit_01 100 100 .8
	End


End



#############################################################

End