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
		Bhvr	:Female_Shields_Talsorian1.bhvr
		Anim	CustomShield_Talsorian_01
	End
End

#############################################################

End