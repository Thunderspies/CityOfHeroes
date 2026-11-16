#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 65

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part	:ChargeUp_Streaks.part
		Part	:ChargeUp_Dots_Top.part
		Part	:ChargeUp_Dots_Bottom.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		#Part	:ChargeUp_Streaks.part
	End
End

#############################################################

End