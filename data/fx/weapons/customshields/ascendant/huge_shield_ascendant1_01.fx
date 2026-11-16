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
		Bhvr	:Huge_Shield_Ascendant1.bhvr
#		BhvrOverride
#			PositionOffset		-0.5 0.15 0.4
#			PyrRotate		10 0 175
#		End
		Anim	CustomShield_Ascendant1_01
	End
End

#############################################################

End