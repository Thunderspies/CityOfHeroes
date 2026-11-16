#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Eyes
			Type Local
			EName EyeAnchor
			Geom CircleOfThorns
		End

		Event
			AltPiv 1
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset	0.0 0.0 -0.05
			End
			Part :Matter_Eye_L.part
			Part :Eye_Glow.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset	0.0 0.0 -0.05
			End
			Part :Matter_Eye_R.part
			Part :Eye_Glow.part
		End

	End

End
