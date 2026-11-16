#########################################################
##	chill touch hit
FxInfo

############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	darkskirt
		Type	posit #Start
		At	ROOT
		Part	:RotatingCirclesDark.part
		Part	:RotatingCircles.part
		Part	:DarkServentRings.part
		Part	:FlatCircles.part
		Part	:FlatCirclesGlow.part

	End

	Event	

		ENAME	hookupW/Spin
		Type	posit
		At	root
		
		BHVR	:SplatRotate.bhvr

	End


	Event
		EName	darkskirt
		Type	posit #Start
		At	ROOT
		
		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part
	End

#	Event	
#
#		ENAME	BaseTrail
#		Type	posit
#		At	root
#
#		Part1	:CreateDarknessTrail.part
#		Part2	:CreateDarknessTrail2.part
#		
#
#	End
#
End

#
#	Event
#		EName	darkskirt
#		Type	posit #Start
#		At	ROOT
#		
#		Part1	:CreateDarknessPuddleFlat.part
#		Part2	:CreateDarknessPuddle2Flat.part
#	End

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

End			