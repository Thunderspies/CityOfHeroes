#############################################################
## Male_Shields_Talsorian1.fx
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
		Bhvr	:Female_Shield_Rularuu1.bhvr
		Anim	CustomShield_Rularuu_01
	End
End

#############################################################

End