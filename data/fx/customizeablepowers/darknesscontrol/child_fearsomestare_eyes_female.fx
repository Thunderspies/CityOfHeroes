#
FxInfo

	Flags DontSuppress
	Lifespan 60

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Eyes
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
				PositionOffset 0 0.035 -0.1
			End
			Part :Eyes_Glow_Big.part
			Part :Eyes_Glow.part
			Part :Eyes_Beam.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.0 0.035 -0.1
			End
			Part :Eyes_Glow_Big.part
			Part :Eyes_Glow.part
			Part :Eyes_Beam.part

		End

	End

End
