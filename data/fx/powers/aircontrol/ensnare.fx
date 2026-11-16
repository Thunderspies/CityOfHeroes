#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Root
End

Input  
	InpName	Hips
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Ring
		Type	start 
		At	Root
		Part1	:Ensnare.part
		Part2	:Ensnare2.part
		Part3	:EnsnareMist.part
		Part4	:Leaves.part
		Part5	:EnsnareMist2.part
		bhvr	behaviors/Wirlwind.bhvr	
		
	End

	Event
		Type	Local 
		At	Root
		
		#Part1	:EnsnareRing1.part
				
	End

End

Condition
	On 	cycle
	Time 	20
	Chance	.99

	Event
		EName 	Rings
		Type	Local 
		At	Root
		
		Bhvr 	behaviors/WirlwindRing.bhvr
		geom	FX_AirWisp01	
		Lifespan 140 # From behavior file
	End

End



End			