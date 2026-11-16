#                                                          
FxInfo

	Flags DontSuppress NotCompatibleWithAnimalHead


###########################################################

	Condition
		On Time
		Time 20

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
			Part :Jaunt_Flash_Head_Big.part
			Part :Jaunt_Glow_Head_Big.part
		End

	End

	Condition
		On Time
		Time 25

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.08 0.225 0.25
			End
			Part :Jaunt_Flash_Head_Big.part
			Part :Jaunt_Glow_Head_Big.part
		End

		Event
			AltPiv 1
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.08 0.225 0.25
			End
			Part :Eyes_Glow_Round_Big.part
			Part :Eyes_Glow_Slanted_Big.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.08 0.225 0.25
			End
			Part :Eyes_Glow_Round_Big.part
			Part :Eyes_Glow_Slanted_Big_Inverted.part
		End

	End

End
