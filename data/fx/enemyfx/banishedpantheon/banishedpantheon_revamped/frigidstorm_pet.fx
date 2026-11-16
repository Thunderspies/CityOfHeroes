#
FxInfo

	LifeSpan 250

###########################################################

	Condition
		On Time
		Time 0

		Event
			At T_Root
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 30 0
			End
			Part :FrigidStorm_Shards.part
			Part :FrigidStorm_Ice.part
			LifeSpan 240
		End

		Event
			At T_Root
			Type Local
			BhvrOverride
				PositionOffset 0 60 0
			End
			Part :FrigidStorm_Dirt.part
			Part :FrigidStorm_Dirt_Alpha.part

			LifeSpan 240
		End

	End

End
