#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

#Condition
#	Event
#		EName 	Prime
#		Type	SetLight
#		Bhvr	:lightPulse.bhvr
#		LifeSpan 90
#	End
#End	

Condition
	On Cycle
	Time 30
	Event
		EName 	Prime
		Type	SetLight
		Bhvr	:lightPulseclamp.bhvr
		LifeSpan 60
	End


#	Event
#		EName 	Prime
#		At		Origin
#		Type	Posit
#		Geom	FX_ICEWEAPON
#		LifeSpan 60
#	End

End			


END