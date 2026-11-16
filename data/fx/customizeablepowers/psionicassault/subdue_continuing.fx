#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input  
	InpName	head
End   

Input  
	InpName	root
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound PsionicsTealHit_loop 60 60 .36
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
		At	head
		
#		part1	:TealHitMist.part
		part2	CustomizeablePowers\PsionicAssault\PurpleInfuseRings.part
		Part4	CustomizeablePowers\PsionicAssault\QuestionMark.part
		Part5	CustomizeablePowers\PsionicAssault\NumberSign.part
		Part3	CustomizeablePowers\PsionicAssault\PercentSign.part
			
	End
	
	Event
		EName	facerings1
		Type	local
		At	Head
		
		Part1	CustomizeablePowers\PsionicAssault\PsionicBubblesHit.part
		Part2	CustomizeablePowers\PsionicAssault\HitRaysBig2.part
		Part4	CustomizeablePowers\PsionicAssault\HitStreaksBig2.part
		Part5	CustomizeablePowers\PsionicAssault\Asterick.part
	End

	Event
		EName	facerings2
		Type	local
		At	root
		
		geom	RootToHeadAdjustment
		
	End

	Event
		EName	facerings3
		Type	local
		At	facerings2
		altpiv	1
		Part1	CustomizeablePowers\PsionicAssault\PurpleInfuseRingsDown.part
		
	End

End

End	


			