#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	root
End

Input  
	Inpname	origin
End

LifeSpan	63

#################################################################################

Condition
	On 	TriggerBits
	TriggerBits	Death

	Event
		EName 	lazer
		Type	Local 
		At	root
				
		Geom	Centerdummy
		lifespan	63

	End


	Event
		EName 	lazer2
		Type	Local
		At	Origin 
		
		Bhvr	behaviors/vanish.bhvr
		Flags	AdoptParentEntity
		LifeSpan	63
	End


	Event
		Type	local
		At	lazer
		Altpiv	1
				
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		LifeSpan	30
		Sound elecballhit 80 80 .8
		LifeSpan	63
	End

	Event
		Type	local
		At	lazer
		Altpiv	1

		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		
		LifeSpan	63
	End

End


End