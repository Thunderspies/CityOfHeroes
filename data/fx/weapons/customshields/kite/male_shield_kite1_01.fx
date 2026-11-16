#############################################################
## Costume Anim Entity - Shield - Roman 1
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
		Bhvr	:Male_Shield_Kite1.bhvr
		Anim	CustomShield_Kite1_Front
		ChildFX	:Child_Kite1_Back.fx
	End
End

#############################################################

End