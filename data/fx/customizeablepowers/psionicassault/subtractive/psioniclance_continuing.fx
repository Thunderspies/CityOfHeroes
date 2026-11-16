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
		
		part1	CustomizeablePowers\PsionicAssault\TealHitMist.part
		part2	CustomizeablePowers\PsionicAssault\PurpleInfuseRings.part
		Part4	CustomizeablePowers\PsionicAssault\QuestionMark.part
		Part5	CustomizeablePowers\PsionicAssault\NumberSign.part
		Part3	CustomizeablePowers\PsionicAssault\PercentSign.part
		Sound teal_loop 50 50 .4
			
	End
	
	Event
		EName	facerings2
		Type	local
		At	Head
		
		Part1	CustomizeablePowers\PsionicAssault\PsionicBubblesHit.part
		Part2	CustomizeablePowers\PsionicAssault\HitRaysBig2.part
		Part4	CustomizeablePowers\PsionicAssault\HitStreaksBig2.part
		Part5	CustomizeablePowers\PsionicAssault\Asterick.part
	End

End

End	


			