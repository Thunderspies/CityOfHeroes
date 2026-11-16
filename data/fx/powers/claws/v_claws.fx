#############################################################
## V_Claws.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	WepR
		BHVR	Behaviors/ClawsFade.bhvr
		Geom	VillianClawsR
		Sound Claw1d 100.0 100.0 .7
	End

	Event
		Type	Local
		At	WepL
		BHVR	Behaviors/ClawsFade.bhvr
		Geom	VillianClawsL
	End
End

#############################################################

End