#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	90

Input  
	InpName	chest
End    

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	chest
		Part	:Ghosts.part
		
		LifeSpan	10
	End

	Event
		Type	start
		At	chest

		Part1	:BlindingLightFlash.part
		#Part4	:BlindingRingFlash.part
		Part5	:FlashStreaks.part
		Sound flashgrenade2 80 80 1.0
		
		
		LifeSpan	70
	End
	
	Event
		Type	start
		At	chest

		#Part1	:BlindingLightFlash.part
		Part4	:BlindingRingFlash.part
		#Part5	:FlashStreaks.part
		LifeSpan	80
	End

	Event
		Type	start
		At	chest

		
		Part2	:BlindingLightRays.part
		
		LifeSpan	20
	End
	

End

Condition
	On 	Time
	Time 	6

	Event
		Type	start
		At	chest

		Part1	:BlindingLightFlash2.part
		
		
	End

End


Condition
	On 	Time
	Time 	12

	Event
		Ename	Dots
		Type	start
		At	chest

		Part1	:BlindingLightFlash3.part
		Part2	:BlindingLightFlashDots.part
		Part3	:BlindingFlashHitCloud.part
		Part	:Poultergueist.part
	End

End    


Condition
	On 	Time
	Time 	33
	Event
		Type	Destroy
		EName	Dots


	End

End


End	


			