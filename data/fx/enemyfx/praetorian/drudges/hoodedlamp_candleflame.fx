#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName CandleFlame
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Scale 0.1 0.1 0.1
			End
			Part :HoodedLamp_flame.part
			Part :HoodedLamp_flame_Core.part
			Part :HoodedLamp_Glow_Core.part
			Part :HoodedLamp_Glow_Add.part
			Part :HoodedLamp_Glow_Trail.part
			Part :HoodedLamp_Glow.part
		End

		Event
			At Chest
			Type Local
			EName CandleAdd
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				#Scale .1 .1 .1
				PositionOffset 0 -0.155 0
				StartColor 255 220 150
			End
			Geom LampCandleAdd
		End

	End

End
