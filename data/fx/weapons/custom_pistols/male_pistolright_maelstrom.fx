#############################################################
## Male_PistolRight_Maelstrom.fx
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
		Anim	CustomWeapon_PistolRight_Maelstrom
	End
End

#############################################################

End