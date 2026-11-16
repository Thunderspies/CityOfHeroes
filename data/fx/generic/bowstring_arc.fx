#############################################################
## FX System Name
#############################################################

FxInfo


LifeSpan 125

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
		LifeSpan	36
		LookAt	Bow_node_R

	End

End
################## Pull String fx  ########################
Condition
	On 	Time
	Time 	36

	Event
		EName	String_R
		Type	Posit
		At	Bow_node_R
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LifeSpan	71
		LookAt	WepR

	End

	Event
		EName	String_L
		Type	Posit
		At	Bow_node_L
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LifeSpan	71
		LookAt	  WepR

	End

End


################# Post Fake dead string #######################

Condition
	On 	Time
	Time 	108

	Event
		EName	String_dead
		Type	Posit
		At	Bow_node_L
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	Bow_node_R

	End

End



End



		