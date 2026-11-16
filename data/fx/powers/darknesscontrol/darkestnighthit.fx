#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	100

Input  
	InpName	chest
End    

#########################################################

Condition
	On 	Time
	Time 	0

	

	Event
		ENAME   Tendrills
		Type	start 
		At	chest
		Part1	:ChillOfNightTendril.part
		#Part2	:DarkBlastHitPuddle2.part
		
	End

	Event

		EName 	Hit
		Type	local 
		At	chest
		
		part1	:DefenceHitMist.part
		part2	:DefenceHitMist2.part
		

	End
	
End

Condition
	On 	Time
	Time 	75

	Event
		EName 	Tendrills
		Type	Destroy
		
	End
	

End

End	


			