#############################################################
## CustomWeapon_Mace.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Sound maceout 60 60 .5
	End
End

#############################################################

End