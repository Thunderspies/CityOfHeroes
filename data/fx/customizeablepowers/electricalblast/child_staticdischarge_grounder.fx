#############################################################
## Child_StaticDischarge_Grounder.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	groundspot
		Type	start
		At	Root
		bhvr	:mu_Elec_Hit_Groundspot.bhvr
	End

	Event
		EName	groundwalker
		Type	posit
		At	chest
		bhvr	:mu_Elec_Hit_Grounder.bhvr
		Magnet	groundspot
	End


	Event
		Type	Posit
		At	groundwalker
		Part	:mu_Elec_Grounder.part
		Part	:mu_Elec_Grounder2.part
		lifespan 40
	End

#############################################################

	Event
		EName	groundspot2
		Type	start
		At	Root
		bhvr	:mu_Elec_Hit_Groundspot.bhvr
	End

	Event
		EName	groundwalker2
		Type	posit
		At	head
		bhvr	:mu_Elec_Hit_Grounder.bhvr
		Magnet	groundspot2
	End


	Event
		Type	Posit
		At	groundwalker2
		Part	:mu_Elec_Grounder.part
		Part	:mu_Elec_Grounder2.part
		lifespan 40
	End

#############################################################

	Event
		EName	groundspot3
		Type	start
		At	Root
		bhvr	:mu_Elec_Hit_Groundspot.bhvr
	End

	Event
		EName	groundwalker3
		Type	posit
		At	hips
		bhvr	:mu_Elec_Hit_Grounder.bhvr
		Magnet	groundspot3
	End


	Event
		Type	Posit
		At	groundwalker3
		Part	:mu_Elec_Grounder.part
		Part	:mu_Elec_Grounder2.part
		lifespan 40
	End
End

#############################################################

End