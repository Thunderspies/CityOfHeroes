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
				
		Part1	:DarknessBlastHitTenticles.part
		Part2	:DarknessBlastHitSmoke.part
		Sound misamaburst 80 80 .8
						
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


			