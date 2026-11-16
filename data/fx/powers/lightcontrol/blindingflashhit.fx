#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	100

Input  
	InpName	Hair
End    

Condition
	On 	Time
	Time 	0

		
	Event
		Type	local
		At	Hair

		Part1	:BlindingLightFlashHitDots.part
		Part2	:BlindingFlashHitCloud.part
	End


End    


End	


			