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

		Part1	:BlindingLightFlash.part
		Part2	:BlindingLightRays.part
		Part4	:BlindingRingFlash.part
		Part5	:FlashStreaks.part
		
	End

End

Condition
	On 	Time
	Time 	6

	Event
		Type	local
		At	Hair

		Part1	:BlindingLightFlash2.part
		
		
	End

End


Condition
	On 	Time
	Time 	12

	Event
		Ename	Dots
		Type	local
		At	Hair

		Part1	:BlindingLightFlash3.part
		Part2	:BlindingLightFlashDots.part
		Part3	:BlindingFlashHitCloud.part
		
	End

End    


Condition
	On 	Time
	Time 	60
	Event
		Type	Destroy
		EName	Dots


	End

End


End	


			