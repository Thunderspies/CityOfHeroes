#############################################################
## AgainstAllOdds_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	150

	Event
		Ename	BodyAura_Initial
		Type	Local
		At	Origin
		ChildFX	:Child_AgainstAllOdds_Continuing_Pulse.fx
		Lifespan 60
	End
End

#############################################################

End