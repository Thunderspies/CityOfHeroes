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
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		Sound blackstar1 100 100 .8
		PMagnet	LarmR
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		PMagnet	LarmL
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
		Part1	:BlackStarRing.Part
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
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		PMagnet	WepR
	End

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		PMagnet	WepL
	End
End

Condition
	On 	Time
	Time	50

	Event
		EName 	HitFake
		Type	local
		At	chest
		Bhvr	Behaviors\CameraShakeDarkCast.bhvr
		Part1	:Blast.part
		Part2	:Blast.part
		Part3	:BlackStarCore.part
	End

	Event
		EName 	HookupPoint
		Type	start
		At	GeomRing
		AltPiv	1
		Bhvr 	Behaviors\DarkScale5.bhvr
		Geom	FX_DarkWave
		LifeSpan	8
	End

	Event
		EName 	HookupPoint
		Type	start
		At	GeomRing
		AltPiv	1
		Bhvr 	Behaviors\DarkScale_5.bhvr
		Geom	FX_DarkWave
		LifeSpan	8
	End

	Event
		EName 	HookupPoint
		Type	start
		At	GeomRing
		AltPiv	1
		Bhvr 	Behaviors\DarkScale_a5.bhvr
		Geom	FX_DarkWave
		LifeSpan	8
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	POWERS\CosmicPowers\Nictus\NictusNova\PeaceBringerTest.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr    POWERS\CosmicPowers\Nictus\NictusNova\Infernobubble.bhvr
		Bhvroverride
			Scale			0.1 0.1 0.1
			ScaleRate		1.05 1.05 1.05
			EndScale		2.0 2.0 2.0
		End
		Part	:BlackstarTendrilsBlack.part
		Part	:BlackStarSmoke.part
		Part	:BlackStarSmokeGrey.part
		Part	:BlackstarTendrilsGrey.part

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

Condition
	On 	Time
	Time	45

	Event
		EName 	Hand1
		Type	Destroy
	End

	Event
		EName 	Hand2
		Type	Destroy
	End

	Event
		EName 	Hand3
		Type	Destroy
	End

	Event
		EName 	Hand4
		Type	Destroy
	End
End

#############################################################

End