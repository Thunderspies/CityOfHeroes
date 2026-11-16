#########################################################
## Red Electric bolts jumping from target into ground
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	40

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		EName	groundspot
		Type	start
		At	Root
		#Geom    Testing_Target
		bhvr	:mu_Elec_Hit_Groundspot.bhvr
		 
	End

		Event
		EName	groundwalker
		Type	posit
		At	chest
		#Geom    Testing_Target
		bhvr	:mu_Elec_Hit_Grounder.bhvr
		Magnet	groundspot
	End

	
	Event
		Type	Posit 
		At	groundwalker
		Part1	:mu_Elec_Grounder.part
		Part2	:mu_Elec_Grounder2.part
#		Sound	lightningbolthit 100 30 .7
		lifespan 40
#		Magnet	Root
	End


	Event
		EName	groundspot2
		Type	start
		At	Root
		#Geom    Testing_Target
		bhvr	:mu_Elec_Hit_Groundspot.bhvr
	End

		Event
		EName	groundwalker2
		Type	posit
		At	head
		#Geom    Testing_Target
		bhvr	:mu_Elec_Hit_Grounder.bhvr
		Magnet	groundspot2
	End

	
	Event
		Type	Posit 
		At	groundwalker2
		Part1	:mu_Elec_Grounder.part
		Part2	:mu_Elec_Grounder.part
#		Sound	lightningbolthit 100 30 .7
		lifespan 40
#		Magnet	Root
	End

		Event
		EName	groundspot3
		Type	start
		At	Root
		#Geom    Testing_Target
		bhvr	:mu_Elec_Hit_Groundspot.bhvr
	End

		Event
		EName	groundwalker3
		Type	posit
		At	hips
		#Geom    Testing_Target
		bhvr	:mu_Elec_Hit_Grounder.bhvr
		Magnet	groundspot3
	End

	
	Event
		Type	Posit 
		At	groundwalker3
		Part1	:mu_Elec_Grounder.part
		Part2	:mu_Elec_Grounder.part
#		Sound	lightningbolthit 100 30 .7
		lifespan 40
#		Magnet	Root
	End

End

End
