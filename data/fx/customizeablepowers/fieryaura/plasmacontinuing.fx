#########################################################
##
##
FxInfo

Flags	IsArmor

#####################################################################################
##Lens Flares
#####################################################


Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		#Sound	Stone4 100 30 .96
	End
End

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	origin
		Sound Mineral_loop 70 70 .72
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	origin
		Sound plasmafire_loop 70 70 .55
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

####################################################################################################


End
