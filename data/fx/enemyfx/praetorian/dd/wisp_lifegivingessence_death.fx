#
FxInfo

	Flags DontSuppress

	LifeSpan 35


###########################################################

	Condition
		On Time
		Time 5

		Event
			At Head
			Type Local
			EName Explosion
			Flags adoptparententity
			Part :Wisp_Streaks_Exploding.part
			Part :Wisp_Streaks_Exploding_Additive.part
			Part :Wisp_Swirls_Exploding.part
			Part :Wisp_Border_Exploding.part
			Part :Wisp_Smoke.part
			Part :Wisp_Smoke_Lingering.part
			LifeSpan 30
		End

	End

End
