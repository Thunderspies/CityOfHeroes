#
FxInfo

	Flags DontSuppress

	LifeSpan 45


###########################################################

	Condition
		On Time
		Time 15

		Event
			At Hips
			Type Local
			EName Explosion
			Flags adoptparententity
			Part :VoidOrb_Streaks_Exploding.part
			Part :VoidOrb_Streaks_Exploding_Additive.part
			Part :VoidOrb_Swirls_Exploding.part
			Part :VoidOrb_Border_Exploding.part
			Part :VoidOrb_Smoke.part
			Part :VoidOrb_Smoke_Lingering.part
			LifeSpan 30
		End

	End

End
