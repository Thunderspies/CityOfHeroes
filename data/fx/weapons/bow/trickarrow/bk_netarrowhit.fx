#########################################################
##	template

FxInfo

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	T_chest
		Geom	NetArrow_Hold
		Bhvr	:NetArrowHoldSpin.bhvr
		LifeSpan	12

	End

End

Condition
	On 	Time
	Time 	3


	Event
		Type	Local
		At	T_chest
		Geom	NetArrow_Hold
		Bhvr	:NetArrowHoldSpin.bhvr
		LifeSpan	12

	End

End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	T_chest
		Geom	NetArrow_Hold
		Bhvr	:NetArrowHold.bhvr
		

	End
	
	Event
		Type	Local
		At	T_chest
		Geom	NetArrow_Hold
		Bhvr	:NetArrowHold2.bhvr
		

	End
	
End

End

		