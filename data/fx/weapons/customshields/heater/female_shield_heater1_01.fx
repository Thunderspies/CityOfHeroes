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
		Bhvr	:Female_Shield_Heater1.bhvr
		Anim	CustomShield_Heater1_Front
		ChildFX	:Child_Heater1_Back.fx
	End
End

#############################################################

End