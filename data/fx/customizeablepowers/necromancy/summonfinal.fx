#########################################################
##	chill touch hit
FxInfo
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
		Part1	CustomizeablePowers\Necromancy\DarkHand.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmber.part
		LifeSpan	50

	End

	Event

		ENAME	Base
		Type	local
		At	WepL

		Part1	CustomizeablePowers\Necromancy\DarkHand.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmber.part
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

		Bhvr	CustomizeablePowers\Necromancy\DarkScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave
		Sound miasmaburst 100 100 .6

		lifeSpan	80
	End

	Event

		ENAME	hookupW/Spin
		Type	posit
		At	T_root

		BHVR	CustomizeablePowers\Necromancy\SplatRotate.bhvr

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

		Part	CustomizeablePowers\Necromancy\CreateDarknessTrail.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessTrail_Subtractive.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessTrail2.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessTrail2_Subtractive.part
		Sound miasma_loop 100 100 .5

		lifeSpan	20

	End

	Event
		EName	darkskirt
		Type	Start
		At	T_root

		Part	CustomizeablePowers\Necromancy\2CreateDarknessPuddle2.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessPuddle2Flat_Subtractive.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessPuddle2Flat.part
		Part	CustomizeablePowers\Necromancy\2CreateDarknessPuddle.part

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

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset2.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset3.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset4.bhvr

		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	33
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset5.bhvr

		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

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

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset.bhvr

		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	34
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset2.bhvr

		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	34
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset3.bhvr

		#part	:SmallFearMist.part
		#part	:SmallFearMist2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	35
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset4.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	36
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset5.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

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

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	36
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset2.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	35
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset3.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	38
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset4.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

		Lifespan	130
	End

End

Condition
	Time 	36
	Chance	.9

	Event
		Type	local
		At	T_root

		Bhvr	CustomizeablePowers\Necromancy\DarkSummonStartOffset5.bhvr

		Part	CustomizeablePowers\Necromancy\SmallFearTenticles.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\SmallFearTenticles2_Subtractive.part

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril2.part

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

		BHVR	CustomizeablePowers\Necromancy\DarktendrilInDark2.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	60
			SecondaryTint	20
		End
		Splat	DarknessSplat_White.tga

		LifeSpan	10
	End

	Event

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\Necromancy\DarktendrilA.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	60
			SecondaryTint	20
		End
		Splat	DarknessSplat_White.tga

		LifeSpan	10

	End

	Event

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\Necromancy\DarktendrilInA.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	20
			SecondaryTint	60
		End
		Splat	DarknessSplat2_White.tga

		LifeSpan	10
	End

	Event

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\Necromancy\DarktendrilInDark.bhvr
		BhvrOverride
			StartColor	255 255 255
			PrimaryTint	20
			SecondaryTint	60
		End
		Splat	DarknessSplat2_White.tga

		LifeSpan	10
	End

End

End