#############################################################
## FX_QuickFadeIn.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Bhvr	WEAPONS\Custom_Pistols\QuickFadeIn.bhvr
		Flags	AdoptParentEntity
	End
End

#############################################################

End