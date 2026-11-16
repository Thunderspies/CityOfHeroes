#
FxInfo

	Flags DontSuppress NotCompatibleWithAnimalHead


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Head
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
				PositionOffset -0.08 0.2 -0.15
			End
			Part CustomizeablePowers\DarknessControl\Eyes_Glow_Round_Small.part
			Part CustomizeablePowers\DarknessControl\Eyes_Glow_Slanted_Small.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.08 0.2 -0.15
			End
			Part CustomizeablePowers\DarknessControl\Eyes_Glow_Round_Small.part
			Part CustomizeablePowers\DarknessControl\Eyes_Glow_Slanted_Inverted_Small.part
		End

	End

End
