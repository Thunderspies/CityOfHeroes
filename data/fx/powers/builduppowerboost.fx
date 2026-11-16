#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

#########################################################

Condition
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound aim4 60 60 .6
		End
end


Condition
	On Time 
	Time 0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part	:ChestLevelUpRaysThinFaint.part
		Part1	:MultiUpGlow.part
		Part2	:MultiUpSpecks2.part
		#Sound	clawMulti3 100 30 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 10
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:MultiUpSpecks.part
		Part2	:MultiUpRing01.part
		Part3	:MultiUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 20
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:MultiUpSpecks.part
		Part3	:MultiUpRing02.part
		Part4	:MultiUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 30
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:MultiUpSpecks2.part
		Part2	:MultiUpRingSlow.part
		Part3	:MultiUpRing03.part
		Part4	:MultiUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

End


Condition
	On Time 
	Time 13

	Event
		EName	pow2
		Type 	Destroy
			
	End

End

Condition
	On Time 
	Time 75

	Event
		EName	all
		Type 	Destroy
			
	End

End

	

End	

	