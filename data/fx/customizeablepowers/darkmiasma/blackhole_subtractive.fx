#############################################################
## BlackHole_Subtractive.fx
#############################################################


FxInfo
LifeSpan	180

##################################
Condition
	On	Time
	Time	0

	Event

		ENAME	hookupW/Spin
		Type	posit
		At	T_root

		BHVR	CustomizeablePowers\DarkMiasma\SplatRotate.bhvr
		Sound Tar_loop 80 80 .53
	End

	Event

		ENAME	BaseTrail
		Type	start
		At	T_root

		Bhvr	CustomizeablePowers\DarkMiasma\BlackHoleSplat.bhvr
		BhvrOverride
			PrimaryTint	50
			SecondaryTint	0
		End
		splat	Generic_Ring.tga
		LifeSpan	10
	End

	Event

		ENAME	BaseTrail
		Type	start
		At	T_root

		Part	CustomizeablePowers\DarkMiasma\CreateDarknessPuddle3_Subtractive.part

	End

End

Condition
	On	cycle
	Time	25
	Chance	.5

	Event

		ENAME	BaseRings
		Type	start
		At	T_root

		Bhvr	CustomizeablePowers\DarkMiasma\BlackHoleSplat.bhvr
		BhvrOverride
			PrimaryTint	0
			SecondaryTint	50
		End
		splat	Generic_Ring.tga
		LifeSpan	10
	End


End

Condition
	On	cycle
	Time	15

	Event

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\DarkMiasma\DarktendrilInDark.bhvr
		BhvrOverride
			PrimaryTint	70
			SecondaryTint	0
		End
		Splat	DarknessSplat_White.tga

		LifeSpan	30
	End

	Event

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\DarkMiasma\Darktendril.bhvr
		BhvrOverride
			PrimaryTint	0
			SecondaryTint	70
		End
		Splat	DarknessSplat_White.tga

		LifeSpan	30

	End

	Event

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\DarkMiasma\DarktendrilIn.bhvr
		BhvrOverride
			PrimaryTint	100
			SecondaryTint	0
		End
		Splat	DarknessSplat2_White.tga

		LifeSpan	30
	End

	Event

		ENAME	BaseTrail2
		Type	start
		At	hookupW/Spin

		BHVR	CustomizeablePowers\DarkMiasma\DarkSplat2.bhvr
		BhvrOverride
			PrimaryTint	50
			SecondaryTint	0
		End
		Splat	Ember02.tga
		LifeSpan	30
	End

End

Condition
	On	Time
	Time	20

	Event
		ENAME	Base1
		Type	local
		At	WepR

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Lifespan	90

	End

	Event

		ENAME	Base2
		Type	local
		At	WepL

		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		Lifespan	90

	End


End

#############################################################

End