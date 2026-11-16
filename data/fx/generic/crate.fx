#############################################################
## crate.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors\fadein.bhvr
		Geom	WepR_Crate2_Sm_Wood
	End
End

#############################################################

End