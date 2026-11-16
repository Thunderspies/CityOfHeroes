#########################################################
##	forceBubble
########################################################
FxInfo


Input  
	Inpname	Hips
End

Input  
	Inpname	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	ManacleCage
		Type	Local    #start 
		At	Root

		
		Bhvr	behaviors/ForceCageSpin.bhvr
		
		Geom	ForceCage
		
		
	End


	Event
		EName 	ManacleCageParticles
		Type	Local    #start 
		At	Root

		Part1	:ForceCageRays.part
		Part2	:ForceCageBase.part		
		Part3	:ForceRing2.part
		Part4	:ForceRingRising.part
		Part5	:ForceCageFlaws.part
	End

End

End
