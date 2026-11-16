#                                                          
FxInfo

	Flags DontSuppress

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			BhvrOverride
				PositionOffset 0 19 0
			End
			Part :Spawn_Dust.part
			Part :Spawn_Dust_Thick.part
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 89

		Event
			At Origin
			Type Start
			BhvrOverride
				PositionOffset 0 -2.0 0.0
			End
			Part :Impale_Shards_Black.part
			LifeSpan 2
		End

	End

End
