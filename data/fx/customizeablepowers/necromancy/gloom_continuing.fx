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
		Sound miasma_loop 55 55 .55
#		bhvr	behaviors/soundFade3.bhvr
#		Flags	PowerLoopingSound
#		Lifespan 420
	End
End


Condition
	On 	Time
	Time 	0

	

	Event
		ENAME   Tendrills
		Type	Local 
		At	chest
		Part1	CustomizeablePowers\Necromancy\ChillOfNightTendril.part
		Part2	CustomizeablePowers\Necromancy\ChillOfNightTendril_Subtractive.part
		#Sound	miasma_loop 55 30 .55
		
	End

	Event

		EName 	Hit
		Type	local 
		At	chest
		
		Part	CustomizeablePowers\Necromancy\DefenceHitMist2_Subtractive.part
		part1	CustomizeablePowers\Necromancy\DefenceHitMist.part
		part2	CustomizeablePowers\Necromancy\DefenceHitMist2.part
		

	End
	

End

End	


			