#############################################################
## SmokeBomb_NoDelay.fx
#############################################################

FxInfo

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash3
		Type	Start
		Sound CoyoteTravel_Transformation_01 100 100 1
		Lifespan 5
	End

	Event
		EName 	Flash3
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		part	:DustCloud.part
		part	:DustCloud_Streaks.part
		Part	:Flash.part
		Lifespan 5
	End
End

Condition
	On 	DEATH

	Event
		EName 	Flash3
		Type	Start
		Sound CoyoteTravel_Transformation_01 100 100 1
		Lifespan 5
	End

	Event
		EName 	Flash3
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		part	:DustCloud.part
		part	:DustCloud_Streaks.part
		Part	:Flash.part
		Lifespan 5
	End
End


#############################################################

End