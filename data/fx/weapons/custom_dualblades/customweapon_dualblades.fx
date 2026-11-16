#########################################################
## CustomWeapon_DualBlades.fx
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Sound swordshing 100 100 .7
	End
End

#############################################################

End