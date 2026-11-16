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

#


############################################################

Condition
	On 	Time
	Time 	10

#	Event	
#
#		ENAME	hookupW/Spin
#		Type	posit
#		At	root
#		
#		BHVR	:SplatRotate.bhvr
#
#	End

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


#Condition
#	On	cycle
#	Time	15
#
#	Event	
#
#		ENAME	BaseTrail
#		Type	start
#		At	hookupW/Spin
#		
#		BHVR	:DarktendrilInDark2.bhvr
#
#		Splat	DarknessSplat.tga
#
#		LifeSpan	30
#	End
#
#	Event	
#
#		ENAME	BaseTrail
#		Type	start
#		At	hookupW/Spin
#		
#		BHVR	:DarktendrilA.bhvr
#
#		Splat	DarknessSplat.tga
#
#		LifeSpan	30
#		
#	End
#
#	Event	
#
#		ENAME	BaseTrail1
#		Type	start
#		At	hookupW/Spin
#		
#		BHVR	:DarktendrilInA.bhvr
#
#		Splat	DarknessSplat2.tga
#
#		LifeSpan	30
#	End
#	
#End

Condition
	On 	Time
	Time 	0

	Event	

		ENAME	Base
		Type	posit
		At	root
		
		Bhvr 	Behaviors/DarkScale3.bhvr
		Geom	FX_DarkWave
		Sound miasmaburst 100 100 .6

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
		Part	:CreateDarknessPuddle_Subtractive_LocalTrail.part
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

#	Event	
#
#		ENAME	BaseTrail
#		Type	posit
#		At	root
#
#		Part1	:CreateDarknessTrail.part
#		Part2	:CreateDarknessTrail2.part
#		Sound	miasma_loop 100 30 .5
#		
#
#	End
#
#	Event
#		EName	darkskirt
#		Type	posit
#		At	ROOT
#		
#		Part1	:CreateDarknessPuddleFlat.part
#		Part2	:CreateDarknessPuddle2Flat.part
#	End

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