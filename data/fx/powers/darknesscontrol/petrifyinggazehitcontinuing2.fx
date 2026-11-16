#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	root
End

Input  
	InpName	chest
End
############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		Sound soot_loop 70 70 .5
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbMist
		Type	Local 
		At	root
		bhvr	behaviors/Rune_spinFastSmall.bhvr
		part1	:DarkessChillMistz.part
		part2	:DarkessChillMist2z.part
		part3	:DarkessChillTenticles.part
		
		Sound miasmaburst 80 80 .8
				
	End

	Event
		ENAME   Tendrills
		Type	local 
		At	chest
		Part1	:ChillOfNightTendril.part
		part2	:PurpleHitMist.part
		part3	:DefenceHitMist2.part
		#Sound	soot_loop 80 60 .5
		

	End
	
End


End			