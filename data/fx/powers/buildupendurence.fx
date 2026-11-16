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
		Sound aim3 60 60 .6
		End
end



Condition
	On Time 
	Time 0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	:BuildUpGlow.part
		Part2	:BuildUpSpecks2.part
		#Sound	clawBuild3 100 30 .8
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
		#Part1	:BuildUpSpecks.part
		Part2	:BuildUpRing01.part
		Part3	:BuildUpRing01a.part
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
		#Part1	:BuildUpSpecks.part
		Part3	:BuildUpRing02.part
		Part4	:BuildUpRing02a.part
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
		#Part1	:BuildUpSpecks2.part
		Part2	:BuildUpRingSlow.part
		Part3	:BuildUpRing03.part
		Part4	:BuildUpRing03a.part
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

	