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
		part1	:DarkessChillMistz.part
		part2	:DarkessChillMist2z.part
		part3	:DarkessChillTenticles2.part
		part3	:DarkessChillTenticles2_Subtractive.part
		Part	:DarkessChillMist2z_Subtractive.part
		
		Sound miasmaburst 70 70 .6
				
	End

	Event
		ENAME   Tendrills
		Type	posit 
		At	chest
		Part1	:ChillOfNightTendril.part
		Part	:DarkBlastHitTendril3_Subtractive.part
		part2	:PurpleHitMist.part
		part3	:DefenceHitMist2.part
		Part	:DarkBlastHitPuddle2_Subtractive.Part
		#Sound	soot2_loop 70 60 .25
		

	End
	
End


End			