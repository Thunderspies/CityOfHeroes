#############################################################
## WaveOfDestruction_Child.fx
#############################################################

FxInfo

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
		Part 	:Flash_TravelUp.part
		Part 	:Air_Dust_TravelUp.part
		Part 	:Air_Dust_TravelUp_Long.part
	End

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			InitialVelocity 0 2 0
			PositionOffset 0 0 5
			PyrRotate 135 0 90
		End
		Part 	:Flash_TravelUp.part
		Part 	:Air_Dust_TravelUp.part
		Part 	:Air_Dust_TravelUp_Long.part
	End

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			InitialVelocity 0 2 0
			PositionOffset -2 0 2
			PyrRotate 135 -22 90
		End
		Part 	:Flash_TravelUp.part
		Part	:Air_Dust_TravelUp.part
		Part 	:Air_Dust_TravelUp_Long.part
	End
End
#######################
End
