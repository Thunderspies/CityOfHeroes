#########################################################
## Red elec blast hitting target
########################################################
FxInfo

Input  
	Inpname	Chest
End

LifeSpan	30

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		Type	Posit 
		At	origin
		ChildFX :mu_LightningBoltHit_grounder.fx
		Sound	mu_cageboltshit 80 80 .7
		lifespan 20
#		Magnet	Root
	End
	Event
		Type	local 
		At	root
		Part1 :mu_Elec_Hit_residual1.part
		lifespan 10
#		Magnet	Root
		Pother	Chest
	End


End

Condition
	On	Time
	Time	1
	Event
		Type	Posit 
		At	root
		ChildFX :mu_LightningBoltHit_grounder.fx
		lifespan 20
	End
	Event
		Type	local 
		At	root
		Part1 :mu_Elec_Hit_residual1.part
		lifespan 10
#		Magnet	Root
		Pother	Chest
	End

End

Condition
	On	Time
	Time	2

	
	Event
		
		Type	start
		At	Origin

		Part1	:mu_shockring1.part
		lifespan	30
	End
	Event
		Type	Posit 
		At	root
		ChildFX :mu_LightningBoltHit_grounder.fx
		lifespan 20
	End
	Event
		Type	local 
		At	root
		Part1 :mu_Elec_Hit_residual1.part
		lifespan 10
#		Magnet	Root
		Pother	Chest
	End


End


End
