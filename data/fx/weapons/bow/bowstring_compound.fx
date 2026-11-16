#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale InheritAlpha DontSuppress IsWeapon

################## nodes  ########################

Condition
	On 	Time
	Time 	0

	Event
		EName   Bow
		Type	Local
		At	WepL
		Anim	New_bow
		Bhvr	Behaviors/turn_bow.bhvr
	End

	Event
		EName   Bow_node_R
		Type	Local
		At	Bow
		AnimPiv	FootR
	End

	Event
		EName   Bow_node_L
		Type	Local
		At	Bow
		AnimPiv	FootL
	End
End

################# Pre Fake dead string #######################

Condition
	On 	Time
	Time 	0

	Event
		EName	String_dead
		Type	Posit
		At	Bow_node_L
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	Bow_node_R
	End
End

#############################################################

End