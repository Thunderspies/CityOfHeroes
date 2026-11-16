#########################################################
##	forceBubble
########################################################
FxInfo

	
#####################
##Sound
############################################

Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound activate16 100.0 100.0 .57

	End

End

Condition
	On	Time	
	Time 	2

	Event
		Type 	Local
		At	Origin
		Sound ForceField17_loop 100.0 100.0 .6

	End

End

Condition
	On	Time	
	Time 	2

	Event
		Type 	Local
		At	Origin
		Sound ForceField18_loop 100.0 100.0 .4

	End

End





#Condition
#	On	Time	
#	Time 	4
#
#	Event
#		Type 	Local
#		At	Origin
#		Sound	forcehands5 80 30 .22
#	End
#
#End
#
#Condition
#	On	Time	
#	Time 	10
#
#	Event
#		Type 	Local
#		At	Origin
#		Sound	ForceField3_loop 100.0 30.0 .42
#		bhvr	behaviors/soundFade3.bhvr
#		Flags	PowerLoopingSound
#		Lifespan 220
#	End
#
#End
#
######################################################################################################3
Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	root
	
		Geom	FX_ForceBubble
		Bhvr 	behaviors/BaseForceFieldScale.bhvr
		Part	:BaseBubbleSparkllies.part
		#LifeSpan	9

	End
	
End

End
