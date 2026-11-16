#########################################################
##	chill touch hit
FxInfo
LifeSpan	200

##################################################################################

Condition
	On	Time
	Time	0

	Event

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Part1	CustomizeablePowers\Necromancy\DarkHand.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmber.part
		LifeSpan	40

	End

	Event

		ENAME	Base
		Type	local
		At	WepL

		Part1	CustomizeablePowers\Necromancy\DarkHand.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmber.part
		LifeSpan	40
	End

End

Condition
	On	Time
	Time	17


	Event
		EName	Prime
		Type	start
		At	wepL
		Bhvr	CustomizeablePowers\Necromancy\MorterLobbProjectile.bhvr

		Part	CustomizeablePowers\Necromancy\MidnightHandTendril_Subtractive.part
		part	CustomizeablePowers\Necromancy\aFearMist.part
		part	CustomizeablePowers\Necromancy\aFearMist2.part
		Part	CustomizeablePowers\Necromancy\aFearTenticles.part
		Part	CustomizeablePowers\Necromancy\aFearTenticles2.part
		part	CustomizeablePowers\Necromancy\aFearMist.part
		part	CustomizeablePowers\Necromancy\aFearMist2.part
		Part	CustomizeablePowers\Necromancy\aFearTenticles.part
		Part	CustomizeablePowers\Necromancy\aFearTenticles2.part
		Part	CustomizeablePowers\Necromancy\MidnightHandTendril.part

		Part	CustomizeablePowers\Necromancy\Bones.part
		Part	CustomizeablePowers\Necromancy\Bones1.part
		Part	CustomizeablePowers\Necromancy\Bones2.part

		Sound MidnightGrasp 70 70 .85
		Magnet	T_Root
		Lookat	T_Root
	End

End


Condition
	On 	PrimeHit


	Event
		Type	Local
		At	T_Root


		Part	CustomizeablePowers\Necromancy\ScatterBones.part
		Part	CustomizeablePowers\Necromancy\ScatterBones1.part
		Part	CustomizeablePowers\Necromancy\ScatterBones2.part

		lifeSpan	9
	End

	Event

		ENAME	Base
		Type	start
		At	T_Root

		Bhvr	CustomizeablePowers\Necromancy\DarkScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave
		Sound miasmaburst 100 100 .6

		lifeSpan	80
	End

	Event

		ENAME	BaseTrail
		Type	start
		At	T_Root

		Part1	CustomizeablePowers\Necromancy\CreateDarknessTrail.part
		Part2	CustomizeablePowers\Necromancy\CreateDarknessTrail2.part
		Sound miasma_loop 100 100 .5

		lifeSpan	20

	End

	Event

		Type	start
		At	T_Root

		Part	CustomizeablePowers\Necromancy\CircleBones.part
		Part	CustomizeablePowers\Necromancy\CircleBones1.part
		Part	CustomizeablePowers\Necromancy\CircleBones2.part

		lifeSpan	24

	End

	Event
		EName	darkskirt
		Type	Start
		At	T_Root


		Part	CustomizeablePowers\Necromancy\2CreateDarknessPuddle2.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessPuddle2Flat_Subtractive.part
		Part	CustomizeablePowers\Necromancy\CreateDarknessPuddle2Flat.part
		Part	CustomizeablePowers\Necromancy\2CreateDarknessPuddle.part

		lifeSpan	20
	End
	Event

		ENAME	hookupW/Spin
		Type	posit
		At	T_root

		BHVR	CustomizeablePowers\Necromancy\SplatRotate.bhvr

		lifeSpan	20
	End
##############################################################################################

	Event
		EName 	Prime
		Type	Destroy

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
			PrimaryTint	70
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
			PrimaryTint	70
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
			PrimaryTint	70
			SecondaryTint	20
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
			PrimaryTint	70
			SecondaryTint	20
		End
		Splat	DarknessSplat2_White.tga

		LifeSpan	10
	End
End

End