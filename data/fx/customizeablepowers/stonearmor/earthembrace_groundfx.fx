#########################################################
## Earth's Embrace
#########################################################

FxInfo

LifeSpan	130

##################################

Condition
	On	Time
	Time	15

	Event
		EName 	OrbMist
		Type	Local
		At	root
		part1	:EarthEmbraceDust.part
		part2	:EarthEmbraceDust2.part
		part3	:EarthEmbraceRocks01.part
		part3	:EarthEmbraceRocks02.part
		LifeSpan	60
	End

	Event
		EName 	OrbMisthook
		Type	Local
		At	root
		geom	EarthEmbrace
		bhvr	behaviors/EarthembraceFastSmall.bhvr
		LifeSpan	100
	End

	Event
		EName 	Orbcracks
		Type	Local
		At	root
		geom	EarthEmbrace
		bhvr	behaviors/EarthembraceCracks.bhvr
		LifeSpan	100
	End

	Event
		EName 	OrbMisthook3
		Type	Local
		At	OrbMisthook
		Altpiv	3
		bhvr	behaviors/EarthembraceStones.bhvr
		Part	:Glow.part

		LifeSpan	89
	End

	Event
		EName 	OrbMisthook4
		Type	Local
		At	OrbMisthook
		Altpiv	4
		bhvr	behaviors/EarthembraceStones.bhvr
		Part	:Glow.part

		LifeSpan	89
	End

	Event
		EName 	OrbMisthook3
		Type	Local
		At	OrbMisthook
		Altpiv	3
		bhvr	behaviors/EarthembraceStones.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_Stone02

		Part	:StoneFallingDust.part
		LifeSpan	90
	End

	Event
		EName 	OrbMisthook4
		Type	Local
		At	OrbMisthook
		Altpiv	4
		bhvr	behaviors/EarthembraceStones.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_Stone02

		Part	:StoneFallingDust.part
		LifeSpan	90
	End

	Event
		EName	Ringcracks
		Type	Local
		At	Root

		Part	:EarthEmbraceCrackRadius.part
		lifespan	70
	End

	Event
		EName 	Orbcrack
		Type	start
		At	Orbcracks
		Altpiv	4

		Part	:EarthEmbraceCrack.part

		LifeSpan	70
	End

	Event
		EName 	Orbcrack2
		Type	start
		At	Orbcracks
		Altpiv	3

		Part	:EarthEmbraceCrack.part
		LifeSpan	70
	End
End


End