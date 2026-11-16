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
		Bhvr	Behaviors\soundFade3.bhvr
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
		Bhvr	Behaviors\soundFade3.bhvr
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
	
		Part1	CustomizeablePowers\DarkMiasma\ChillOfNightTendril.part
		part2	CustomizeablePowers\DarkMiasma\TealHitMist.part
		part3	CustomizeablePowers\DarkMiasma\DefenceHitMist2.part
		

	End
	
End

End	


			