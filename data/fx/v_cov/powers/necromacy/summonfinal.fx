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

LifeSpan	200


############################################################

Condition
	On	Time
	Time	10
		
	Event	

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	50

	End

	Event	

		ENAME	Base
		Type	local
		At	WepL
		
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	50
	End

End

Condition
	On 	Time
	Time 	30

	Event	

		ENAME	Base
		Type	start
		At	T_root
		
		Bhvr 	:DarkScale3.bhvr
		Geom	FX_DarkWave
		Sound miasmaburst 100 100 .6
		
		lifeSpan	80
	End

	Event	

		ENAME	hookupW/Spin
		Type	posit
		At	T_root
		
		BHVR	POWERS\DarknessControl\SplatRotate.bhvr
		
		lifeSpan	70
	End
End

Condition
	On 	Time
	Time 	30
	
	Event	

		ENAME	BaseTrail
		Type	start
		At	T_root

		Part1	powers/DarknessControl/CreateDarknessTrail.part
		Part	powers/DarknessControl/CreateDarknessTrail_Subtractive.part
		Part2	powers/DarknessControl/CreateDarknessTrail2.part
		Part	powers/DarknessControl/CreateDarknessTrail2_Subtractive.part
		Sound miasma_loop 100 100 .5
		
		lifeSpan	20

	End

	Event
		EName	darkskirt
		Type	Start
		At	T_root
		
		Part	:2CreateDarknessPuddle2.part
		Part	powers/DarknessControl/CreateDarknessPuddle2Flat_Subtractive.part
		Part	powers/DarknessControl/CreateDarknessPuddle2Flat.part
		Part	:2CreateDarknessPuddle.part
		
		lifeSpan	20
	End

End

#############################################################################################################################

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset2.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset3.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset4.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset5.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

#############################################################################################################################

Condition
	Time 	35
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	34
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset2.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	34
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset3.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	35
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset4.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	36
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset5.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

#############################################################################################################################

Condition
	Time 	37
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	36
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset2.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	35
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset3.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	38
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset4.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	36
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	Behaviors\DarkSummonStartOffset5.bhvr
		
		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	:SmallFearTenticles.part
		Part	:SmallFearTenticles2.part
		Part	:SmallFearTenticles2_Subtractive.part

		Part	:MidnightHandTendril2.part

		Lifespan	130
	End

End


Condition
	On	cycle
	Time	5
	chance	.9

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	POWERS\DarknessControl\DarktendrilInDark2.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	10
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	POWERS\DarknessControl\DarktendrilA.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	10
		
	End

	Event	

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin
		
		BHVR	POWERS\DarknessControl\DarktendrilInA.bhvr

		Splat	DarknessSplat2.tga

		LifeSpan	10
	End
	
	Event	

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin
		
		BHVR	POWERS\DarknessControl\DarktendrilInDark.bhvr

		Splat	DarknessSplat2.tga

		LifeSpan	10
	End

End

End			