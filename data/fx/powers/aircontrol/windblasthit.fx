#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	chest
End    

LifeSpan	100

Condition
	On 	Time
	Time 	6

		
	Event
		Type	start
		At	chest

		Part1	:levitateHitRing.part
		
	End

End    

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	chest

		Part1	:WindBlastHitPuff.part
		Part2	:WindBlastHitPuff2.part
		Part3	:AirWisp01.part
		Part4	:WindBlastRocks.part
	
	End

End

End	


			