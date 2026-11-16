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
		Bhvr	:Huge_Shield_Cavalier1.bhvr
		Anim	CustomShield_Cavalier2_Front
		ChildFX	:Child_Cavalier2_Back.fx
	End
End

#############################################################

End