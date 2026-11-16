#############################################################
## Costume Anim Entity - Shield - Default
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Anim	CustomShield_Cavalier2_Back
	End
End

#############################################################

End