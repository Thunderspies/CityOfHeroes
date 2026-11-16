#############################################################
## Leaping_NoFX.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Posit
		At	FootR
		Sound Superleap_loop 50.0 50.0 .5
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 100
	End

End


Condition
	On	Time
	Time	0


	Event
		Type	Posit
		At	FootR
		Sound whoosh7 50.0 50.0 .49
	End

End

#############################################################

End