#########################################################
##	Soul Storm
##
FxInfo


LifeSpan  100


################  Start   #####################




Condition
	On Time
	Time 0

	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:soulLights.bhvr
		#Sound	blackhole22 80 60 .6
		LifeSpan 30
	End

End

Condition
	On	Time
	Time	1

	Event
		EName 	Explode 
		Type	Start
		At	Waist
		Part	:soulFlash1.part
		Part	:soulExplode2.part		
		Part	:soulSparks2.part
		Part	:soulCloud2.part
	End

	Event
		EName 	All 
		Type	Destroy
	End

End

End				