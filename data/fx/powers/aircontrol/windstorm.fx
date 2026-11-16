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

LifeSpan	200

Condition
	On 	Time
	Time 	20

	Event
		EName	Hookup
		Type	local
		At	root
		Geom	OverHeadDummy
		
	End


	Event
		EName 	Ring
		Type	local 
		At	Root
		Part1	:Ensnare.part
		Part2	:Ensnare2.part
		Part3	:EnsnareMist.part
		Part4	:Leaves2.part
		Part5	:EnsnareMist2.part
		bhvr	behaviors/Wirlwind.bhvr	
		
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		
		part1	:RainCloudDark.part
		part2	:RainCloudDark2.part
		Part3	:RockHail.part
		Part4	:Mist.part
		
	End

End



End			