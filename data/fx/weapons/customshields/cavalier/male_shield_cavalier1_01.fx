#############################################################
## Costume Anim Entity - Shield - Default
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	:Male_Shield_Cavalier1.bhvr
		Anim	CustomShield_Cavalier1_Front
		ChildFX	:Child_Cavalier1_Back.fx
	End
End

#############################################################

End