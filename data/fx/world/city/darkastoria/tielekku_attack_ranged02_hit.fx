#
FxInfo

	LifeSpan 90


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_TielekkuRanged02_Hit_01 100 100 .8
	End
End


	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			#Sound CoL_LightStaffBolt_Hit_01 100 100 0.8
		End

		Event
			At Chest
			Type Local
			EName Pow
			Part :HitFlash.part
			Part :ProjectileHitFlare.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 4

		Event
			At Chest
			Type Local
			EName Pow
			Part :ProjectileHitFlash_Secondary.part
			Part :ProjectileHitStreaks.part
			Part :ProjectileHitStreaks2.part
			LifeSpan 10
		End

	End

End
