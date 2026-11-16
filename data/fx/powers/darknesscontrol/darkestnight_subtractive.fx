#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	root
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

#LifeSpan	250

############################################################

Condition
	On 	Time
	Time 	5

	Event	

		ENAME	HandR
		Type	local
		At	WepR

		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		Sound miasmaburst 100 100 .6

	End

	Event	

		ENAME	HandL
		Type	local
		At	WepL
		
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
	End
	
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	OrbMist
		Type	Posit 
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFast.bhvr
		
				
	End

	Event
		EName 	OrbStreak
		Type	Posit 
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFaster.bhvr
				
	End

	Event
		EName 	Orb
		Type	Local 
		At	OrbStreak
		altpiv  1
		part1	:DarkessChillMistTrail.part
		part2	:DarkessChillMistTrail2.part
		Sound miasma_loop 100 100 .5
			
		
	End


	Event
		EName	darkskirt
		Type	Posit #Start
		At	root
		
		Part	:SubtractiveFlash1.part
		Part	:CreateDarknessPuddle_Subtractive_Local.part
		Part1	:CreateDarknessPuddle_Subtractive.part
		Part2	:CreateDarknessPuddle2_Subtractive.part
	End

	Event	

		ENAME	BaseTrail
		Type	Posit
		At	root

		Part1	:CreateDarknessTrail_Subtractive.part
		Part2	:CreateDarknessTrail2_Subtractive.part
		

	End

	Event
		EName	darkskirt
		Type	Posit #Start
		At	root
		
		Part1	:CreateDarknessPuddleFlat_Subtractive.part
		Part2	:CreateDarknessPuddle2Flat_Subtractive.part
	End

End

Condition
	On 	Time
	Time 	135

Event
		EName 	OrbStreak2
		Type	Posit 
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFaster.bhvr
				
	End

	Event
		EName 	Orb2
		Type	Local 
		At	OrbStreak2
		altpiv  1
		part1	:DarkessChillMistTrail.part
		part2	:DarkessChillMistTrail2.part
			
		
	End


End
	
Condition
	On 	Time
	Time	90
	Event
		EName 	HandL
		Type	Destroy

	End

	Event
		EName 	HandR
		Type	Destroy

	End
End

End			