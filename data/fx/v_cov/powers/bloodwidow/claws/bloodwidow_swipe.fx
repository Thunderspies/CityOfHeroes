#############################################################
## Blood Widow "Swipe" FX
#############################################################

FxInfo

Lifespan 15

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	Streak
		Type	local
		At	Mystic
		Bhvr	behaviors/Claws.bhvr
		Geom	swoosh
		Lifespan 2
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Type	local
		At	Mystic
		Sound knifewhoosh3 80 80 .6

	End
End

#############################################################

End