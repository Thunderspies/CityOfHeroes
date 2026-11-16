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
				PyrRotate 135 22 90
			End
			Part :Air_Dust_TravelUp.part
			Part :Air_Dust_TravelUp_Long.part
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
				PyrRotate 135 0 90
			End
			Part :Air_Dust_TravelUp.part
			Part :Air_Dust_TravelUp_Long.part
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
				PositionOffset -2 0 2
				PyrRotate 135 -22 90
			End
			Part :Air_Dust_TravelUp.part
			Part :Air_Dust_TravelUp_Long.part
		End

	End

End
