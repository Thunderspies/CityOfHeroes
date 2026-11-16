#########################################################
##	Fire_Ball
##
FxInfo

Condition
	On	Time
	Time	0

	Event
		Type Local
		At T_Root
		Sound Sonic3_loop 100 100 .22
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End
End



End	


			