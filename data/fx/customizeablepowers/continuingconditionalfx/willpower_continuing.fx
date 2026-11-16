#########################################################
##  Willpower - Continuing effect
#########################################################

FxInfo

#########################################################

Condition
	On	Cycle
	Time	40

	Event
		Ename	Hearbeat
		Type	Local
		At	Origin
		ChildFX	:WillPower_Continuing_Heartbeat.fx
		Lifespan	60
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		sound	heart_loop 50 40 .21
		bhvr	behaviors/soundFade3.bhvr
		Lifespan	60
	End
End

#########################################################

End