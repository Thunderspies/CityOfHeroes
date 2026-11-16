#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

	Event
		EName	Effect01
		Type	Local
		At	Root
		Sound 	FirstWard_ShadowyPathsEnter_01 100 100 .65
	End

	Event
		EName	Effect02
		Type	Local
		At	Root
		Sound 	FirstWard_InvertedShadowyComets_Loop 100 100 .07
		bhvr	behaviors/Sound/SoundIn3Out5.bhvr
	End

	Event
		EName	Effect03
		Type	Local
		At	Root
		Sound FirstWard_InvertedShadowyPaths_Loop 100 100 0.6
		bhvr	behaviors/Sound/SoundIn3Out5.bhvr
	End

		Event
			Type Start
			CameraSpace
			EName Effect04
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Redout_Edges_Inverted.part
			Part :Screen_Particles_Inverted.part
		End

		Event
			Type Start
			CameraSpace
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Transition_Swirl_Inverted.part
			Part :Transition_Swirl_Core_Inverted.part
			LifeSpan 30
		End

	End

	Condition
		On Death

		Event
			Type Destroy
			EName Effect02
		End

		Event
			Type Destroy
			EName Effect03
		End

		Event
			Type Destroy
			EName Effect04
		End

		Event
			At Root
			Type Local
			Sound FirstWard_ShadowyPathsExit_01 100 100 0.8
		End

		Event
			Type Start
			CameraSpace
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Transition_Swirl_Inverted.part
			Part :Transition_Swirl_Core_Inverted.part
			LifeSpan 30
		End

	End

End
