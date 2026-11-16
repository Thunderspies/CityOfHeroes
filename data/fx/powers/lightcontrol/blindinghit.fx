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

		Part1	:BlindingLight.part
		Part2	:BlindingLightRing.part
		Part3	:BlindingLightCloud2.part
		Part4	:BlindingLight3.part
		Part5	:Streaks.part
		
	End

	Event
		Type	local
		At	Hair

		Part3	:BlindingLightFlashHitDots.part
		
	End

End    


End	


			