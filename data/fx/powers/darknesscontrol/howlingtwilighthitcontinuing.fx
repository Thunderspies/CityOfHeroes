#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input  
	InpName	chest
End    

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	chest
		Sound Soot2_loop 80 80 .6
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End



Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	chest
		#Sound	Soot2_loop 80 25 .7
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End



Condition
	On 	Time
	Time 	0

	Event

		EName 	Hit
		Type	local 
		At	chest
		#Sound	Soot2_loop 80 25 .7
	
		Part1	:ChillOfNightTendril.part
		part2	:TealHitMist.part
		part3	:DefenceHitMist2.part
		

	End
	
End

End	


			