#############################################################
## CustomWeapon_Bow.fx - Activation FX for Player Bow powers
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress IsWeapon

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Sound Bowout 50 50 .2
	End
End

#############################################################

End