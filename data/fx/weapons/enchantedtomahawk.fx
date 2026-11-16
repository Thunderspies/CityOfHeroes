#############################################################
## Header
#############################################################

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
		Geom	GEO_WepR_Axe_4
	End

	Event
		Type	Local
		At	Prime
		BHVR	:TomahawkOffset.bhvr
		Part	:TomahawkGlow.part
		Part	:TomahawkTendril.part
	End
End

#############################################################

End