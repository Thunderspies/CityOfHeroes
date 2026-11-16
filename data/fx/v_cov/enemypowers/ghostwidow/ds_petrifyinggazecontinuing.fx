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
		Type	posit
		At	root
		Sound soot2_loop 70 70 .4
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
		Type	posit 
		At	root
		bhvr	behaviors/Rune_spinFastSmall.bhvr
		part1	POWERS/DarknessControl/DarkessChillMistz.part
		part2	POWERS/DarknessControl/DarkessChillMist2z.part
		part3	:DarkessChillTenticles.part
		
		Sound miasmaburst 70 70 .6
				
	End

	Event
		ENAME   Tendrills
		Type	posit 
		At	chest
		Part1	POWERS/DarknessControl/ChillOfNightTendril.part
		#part2	POWERS/DarknessControl/PurpleHitMist.part
		#part3	POWERS/DarknessControl/DefenceHitMist2.part
		
		Part	:soulChoke1.part
		Part	:DarkSoulBeamTenticle.part
		#Sound	soot2_loop 70 60 .25
		

	End
	
End


End			