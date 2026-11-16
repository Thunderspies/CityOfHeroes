#
FxInfo

	LifeSpan 100


###########################################################


	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName AirRushAnchor
			BhvrOverride
				InitialVelocity 0 2 0
				PositionOffset 2 0 2
				PyrRotate 90 5 90
			End
			Part :Sand_Dust_TravelUp.part
			Part :Sand_Dust_TravelUp_Long.part
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Root
			Type Local
			EName AirRushAnchor
			BhvrOverride
				InitialVelocity 0 2 0
				PositionOffset 0 0 5
				PyrRotate 90 0 90
			End
			Part :Sand_Dust_TravelUp.part
			Part :Sand_Dust_TravelUp_Long.part
		End

	End

	Condition
		On Time
		Time 4

		Event
			At Root
			Type Local
			EName AirRushAnchor
			BhvrOverride
				InitialVelocity 0 2 0
				PositionOffset -3 0 4
				PyrRotate 90 -5 90
			End
			Part :Sand_Dust_TravelUp.part
			Part :Sand_Dust_TravelUp_Long.part
		End

	End

End
