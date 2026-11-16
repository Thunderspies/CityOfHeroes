#
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################
	Condition
		On Cycle
		Time 1
		Chance 0.1000

		Event
			At Origin
			EName sparks
			Type Local
			BhvrOverride
				#PositionOffset 0 0 0
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_RailRoad_Pike_Sparks.part
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_RailRoad_Pike_Lighting.part
			LifeSpan 30
		End

	End

End
