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

LifeSpan	250

############################################################

Condition
	On 	Time
	Time 	10

	Event	

		ENAME	hookupW/Spin
		Type	posit
		At	root
		
		BHVR	:SplatRotate.bhvr

	End

	Event	

		ENAME	HandR
		Type	local
		At	WepR

		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part

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
	On	cycle
	Time	15

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarktendrilInDark2.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarktendrilA.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
		
	End

	Event	

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarktendrilInA.bhvr

		Splat	DarknessSplat2.tga

		LifeSpan	30
	End
	
End

Condition
	On 	Time
	Time 	20

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

	Event
		EName	darkskirt
		Type	Start
		At	ROOT
#		
#		Part1	:CreateDarknessPuddle.part
#		Part2	:CreateDarknessPuddle2.part
		Sound miasmaburst 100 100 .8
	End

#	Event	
#
#		ENAME	BaseTrail
#		Type	Start
#		At	root
#
#		Part1	:CreateDarknessTrail.part
#		Part2	:CreateDarknessTrail2.part
#		
#
#	End
#
#	Event
#		EName	darkskirt
#		Type	Start
#		At	ROOT
#		
#		Part1	:CreateDarknessPuddleFlat.part
#		Part2	:CreateDarknessPuddle2Flat.part
#	End

	Event	

		ENAME	CloudPillar
		Type	Start
		At	Root
		
#		part1	:HowlingTwilightMist.part
#		part2	:HowlingTwilightMist2.part
		
		
		part1	:HowlingTwilightMist_Subtractive.part
		part2	:HowlingTwilightMist2_Subtractive.part


			Sound miasma_loop 100 100 .5
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