#############################################################
## AvitarDarknessContinuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	120

	Event
		EName 	Prime
		#At	Origin
		Type	SetLight
		BHVR	GENERIC\LowGlow\Darkpulseclamp.bhvr
		LifeSpan 120
	End
End

#############################################################

End