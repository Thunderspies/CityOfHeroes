#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Shillelagh
		Bhvr	:Shillelagh.bhvr
		Sound axeout 70 70 .5
	End
End

End