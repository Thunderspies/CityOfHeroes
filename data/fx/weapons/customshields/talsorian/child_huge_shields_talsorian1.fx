#############################################################
## Child_Male_Shields_Talsorian1.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	Origin
		Bhvr	:Huge_Shields_Talsorian2.bhvr
		Anim	CustomShield_Talsorian_02
	End
End

#############################################################

End