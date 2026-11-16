#############################################################
## New_bow.fx - Activation FX for bow weapons
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress IsWeapon

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		#Bhvr	Behaviors/turn_bow.bhvr
		Anim	New_bow
		Sound Bowout 50 50 .2
	End
End

#############################################################

End