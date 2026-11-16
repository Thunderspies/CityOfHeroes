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
				PositionOffset -0.08 0.225 0.25
			End
			Part :Jaunt_Flash_Head.part
			Part :Jaunt_Glow_Head.part
		End
	End

	Condition
		On	Time
		Time	5

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.08 0.225 0.25
			End
			Part :Jaunt_Flash_Head.part
			Part :Jaunt_Glow_Head.part
		End

		Event
			AltPiv 1
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.08 0.225 0.25
			End
			Part :Eyes_Glow_Round.part
			Part :Eyes_Glow_Slanted.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.08 0.225 0.25
			End
			Part :Eyes_Glow_Round.part
			Part :Eyes_Glow_Slanted_Inverted.part
		End
End


###########################################################

End
