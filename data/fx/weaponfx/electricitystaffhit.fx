#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	50


Input  
	InpName	chest
End    



#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	hit
		Type	Local 
		At	chest
				
		Part1	:ElectricityStaffHit.part
		Part2	:ElectricityStaffHitInvert.part
		
	End


	
End


End	


			