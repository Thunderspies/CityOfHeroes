#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End
	

Lifespan	600
################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	GreenCompass
		Type	local
		At	Root
		
		Bhvr	Behaviors/GreenCompassAlpha.bhvr
		Geom	HudCompassGreen
		
	End

End

End
