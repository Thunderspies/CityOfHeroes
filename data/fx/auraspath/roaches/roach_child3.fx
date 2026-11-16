#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo
#########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE

	Event
		Type	PositOnly
		At	Root
		WHILE	MALE
		Part :Roach_GroundIdle.part
		Part :Roach_Splode.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	Event
		Type	PositOnly
		At	Root
		WHILE	FEMALE
		Part :Roach_GroundIdle.part
		Part :Roach_Splode.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	Event
		Type	PositOnly
		At	Root
		WHILE	HUGE
		Part :Roach_GroundIdle.part
		Part :Roach_Splode.part
	End
End


#########################################################

End