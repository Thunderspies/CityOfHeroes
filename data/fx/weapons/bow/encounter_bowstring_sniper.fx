#########################################################
##	template

FxInfo

Flags InheritAlpha IsWeapon

LifeSpan 130

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
		AnimPiv	LLegR
	End

	Event
		EName   Bow_node_L
		Type	Local
		At	Bow
		AnimPiv	LLegL
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
		LifeSpan	20
		LookAt	Bow_node_R

	End

End
################## Pull String fx  ########################
Condition
	On 	Time
	Time 	20

	Event
		EName	String_R
		Type	Posit
		At	Bow_node_R
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LifeSpan	40
		LookAt	WepR

	End

	Event
		EName	String_L
		Type	Posit
		At	Bow_node_L
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LifeSpan	40
		LookAt	  WepR

	End

End


################# Post Fake dead string #######################

Condition
	On 	Time
	Time 	61

	Event
		EName	String_dead
		Type	Posit
		At	Bow_node_L
		Bhvr	behaviors/XRayBeam.bhvr
		geom	Bow_String
		LookAt	Bow_node_R

	End

End

######## arrow in hand   ###########################


Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Bow\Archery\HandRotate.bhvr
		Geom	NormalArrow
		Sound arrowout2 50 50 .25
		LifeSpan 48
	End
End


Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	WepR
		Sound bowtwang3 54.0 54.0 .5
	End
End

End





