#
FxInfo

	Flags DontSuppress NotCompatibleWithAnimalHead


###########################################################

	Condition
		On Time
		Time 0

		Event
			EName	BeaconAnchor
			Type	Local
			At	Hips
			BhvrOverride
				PositionOffset	0.0 -4.0 0.0
			End
			Sound 	FirstWard_ShadowyOrb1_Loop 50 50 .15
			bhvr	behaviors/Sound/SoundIn3Out5.bhvr
		End

		Event
			At BeaconAnchor
			Type Local
			EName EyeAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			Geom CircleOfThorns
		End

		Event
			AltPiv 1
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset	-0.35 0.15 0.0
			End
			Part :Eyes_Glow_Round_Small.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset	0.35 0.15 0.0
			End
			Part :Eyes_Glow_Round_Small.part
		End

	End

End
