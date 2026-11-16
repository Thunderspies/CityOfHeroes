#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	40

Input  
	InpName	chest
End

#########################################################

Condition
	On 	Time
	Time 	0

	

	Event
		EName 	mouth
		Type	local 
		At	chest
				
		Part1	:CrystalStaffRings.part
		Part2	:CrystalStaffGlow.part
		Part3	:CrystalStaffRays.part
		Part4	:CrystalStaffBalls.part	
		Part5	:CrystalStaffRings1.part
		Sound CrystalHit 80 80 .7
	End

End

Condition
	On 	Time
	Time	40

	Event
		EName 	mouth
		Type	Destroy

	End

End

End	


			