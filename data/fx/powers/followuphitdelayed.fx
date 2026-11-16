#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

Input  
	InpName	Origin
End


#########################################################


Condition
	On Time 
	Time 0
	
	Event
		Type 	local
		At	Origin
		Sound icefollow 80 80 .8
	End

End

Condition
	On Time 
	Time 55
	
	Event
		EName	dummy
		Type 	local
		At	Chest
		Geom	CenterDummy
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 55
	
	Event
		EName	Pow2
		Type 	local
		At	dummy
		altpiv	1
		Part1	:FollowUpGlow.part
		Part2	:FollowUpSpecks2.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 65
	
	Event
		EName	Pow
		Type 	local
		At	dummy
		altpiv	1
		#Part1	:FollowUpSpecks.part
		Part2	:FollowUpRing01.part
		Part3	:FollowUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

End

Condition
	On Time 
	Time 75
	
	Event
		EName	Pow
		Type 	local
		At	dummy
		altpiv	1
		#Part1	:FollowUpSpecks.part
		Part3	:FollowUpRing02.part
		Part4	:FollowUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 85
	
	Event
		EName	Pow
		Type 	local
		At	dummy
		altpiv	1
		#Part1	:FollowUpSpecks2.part
		Part2	:FollowUpRingSlow.part
		Part3	:FollowUpRing03.part
		Part4	:FollowUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

End


Condition
	On Time 
	Time 88

	Event
		EName	pow2
		Type 	Destroy
			
	End

End

Condition
	On Time 
	Time 150

	Event
		EName	all
		Type 	Destroy
			
	End

End

	

End	

	