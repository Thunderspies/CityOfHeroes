#############################################################
## Header
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	GeomRing
		Type	local
		At	Root
		Geom	RootToChestAdjustment
	End

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		Sound blackstar1 100 100 .8
		PMagnet	LarmR
		Lifespan 45
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	LarmL
		Lifespan 45
	End
End

Condition
	On 	Time
	Time 	49

	Event
		EName 	Rings
		Type	start
		At	GeomRing
		AltPiv	1
		Part1	CustomizeablePowers\DarkBlast\BlackStarRing.Part
		LifeSpan	40
	End
End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Hand2
		Type	Local
		At	LarmR

		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		PMagnet	WepR
		Lifespan 30

	End

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		PMagnet	WepL
		Lifespan 30

	End
End

Condition
	On 	Time
	Time	50

	Event
		EName 	HitFake
		Type	local
		At	chest
		Bhvr	CustomizeablePowers\DarkBlast\CameraShakeDarkCast.bhvr
		Part1	CustomizeablePowers\DarkBlast\Blast.part
		Part2	CustomizeablePowers\DarkBlast\Blast.part
		Part3	CustomizeablePowers\DarkBlast\BlackStarCore.part
	End

	Event
		EName 	HookupPoint
		Type	start
		At	GeomRing
		AltPiv	1
		Bhvr	CustomizeablePowers\DarkBlast\DarkScale5.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave		LifeSpan	8
	End

	Event
		EName 	HookupPoint
		Type	start
		At	GeomRing
		AltPiv	1
		Bhvr	CustomizeablePowers\DarkBlast\DarkScale_5.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave		LifeSpan	8
	End

	Event
		EName 	HookupPoint
		Type	start
		At	GeomRing
		AltPiv	1
		Bhvr	CustomizeablePowers\DarkBlast\DarkScale_a5.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave
		LifeSpan	8
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	CustomizeablePowers\DarkBlast\PeaceBringerTest.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\DarkBlast\Infernobubble.bhvr
		Bhvroverride
			Scale			0.1 0.1 0.1
			ScaleRate		1.05 1.05 1.05
			EndScale		2.0 2.0 2.0
		End
		Part	CustomizeablePowers\DarkBlast\BlackstarTendrilsBlack.part
		Part	CustomizeablePowers\DarkBlast\BlackStarSmoke.part
		Part	CustomizeablePowers\DarkBlast\BlackStarSmokeGrey.part
		Part	CustomizeablePowers\DarkBlast\BlackstarTendrilsGrey.part

	End
End

#############################################################

Condition
	On 	Time
	Time	93

	Event
		EName 	HitFake
		Type	Destroy
	End
End

#############################################################

End