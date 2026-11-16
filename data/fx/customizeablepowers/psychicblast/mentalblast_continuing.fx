#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input  
	InpName	head
End    

#########################################################

Condition
	On 	Time
	Time 	0

	Event

		EName 	Hit
		Type	local 
		At	head
		
		part1	CustomizeablePowers\PsychicBlast\TealHitMist.part
		part2	CustomizeablePowers\PsychicBlast\PurpleInfuseRings.part
		Part4	CustomizeablePowers\PsychicBlast\QuestionMark.part
		Part5	CustomizeablePowers\PsychicBlast\NumberSign.part
		Part3	CustomizeablePowers\PsychicBlast\PercentSign.part
		Sound teal_loop 50 50 .4
			
	End
	
	Event
		EName	facerings2
		Type	local
		At	Head
		
		Part1	CustomizeablePowers\PsychicBlast\PsionicBubblesHit.part
		Part2	CustomizeablePowers\PsychicBlast\HitRaysBig2.part
		Part4	CustomizeablePowers\PsychicBlast\HitStreaksBig2.part
		Part5	CustomizeablePowers\PsychicBlast\Asterick.part
	End

End

End	


			