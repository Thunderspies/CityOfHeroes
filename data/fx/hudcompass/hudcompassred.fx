#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End

	
lifespan	600
################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	GreenCompass
		Type	local
		At	Root
		
		Bhvr	Behaviors/GreenCompassAlpha.bhvr
		Geom	HudCompassRed
		
	End
	
End

End
