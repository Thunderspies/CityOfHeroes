#############################################################
## CustAnim_DarkChasm.fx
#############################################################

FxInfo
LifeSpan	350

###############################################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	Hand1
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		Sound DarkPit1 80 80 .78

		PMagnet	LarmL
		LifeSpan	27
	End

	Event
		EName 	Hand2
		Type	Local
		At	LarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part


		PMagnet	WepL
		LifeSpan	27
	End

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		Sound DarkPit1 80 80 .78

		PMagnet	LarmL
		LifeSpan	27
	End

	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part


		PMagnet	WepL
		LifeSpan	27
	End

End


Condition
	On	time
	time	25

	Event
		EName	MysticAnchor
		Type	Local
		At	Head
		Geom	PsionicRigging
		BhvrOverride
			PositionOffset	0.0 0.0 1.5
		End
	End


	Event
		EName 	Prime
		Type	Start
		At	MysticAnchor
		Bhvr	CustomizeablePowers\DarkBlast\MorterLobbprojectile.bhvr

		Part1	CustomizeablePowers\DarkBlast\DarknessRapidTenticles.part
		Part2	CustomizeablePowers\DarkBlast\DarknessRapidTenticlesCore.part
		Part3	CustomizeablePowers\DarkBlast\DarknessRapidCloud.part
		Part4	CustomizeablePowers\DarkBlast\DarknessRapidCloud2.part
		Part5	CustomizeablePowers\DarkBlast\SmallTenticlesHead.part

		Magnet	T_Root
		LookAt	T_Root
	End

End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		EName 	burst
		Type	start
		At	T_Root
		bhvr	CustomizeablePowers\DarkBlast\Infernobubble.bhvr
		Part1	CustomizeablePowers\DarkBlast\PitBust1.part
		Part2	CustomizeablePowers\DarkBlast\PitBust2.part
		Sound grexplo2 80 80 1.0

	End

	Event
		EName	darkskirt
		Type	start #Start
		At	T_ROOT

		Part1	CustomizeablePowers\DarkBlast\CreateDarknessPuddle.part
		Part2	CustomizeablePowers\DarkBlast\CreateDarknessPuddle2.part
		Sound forcefield5_loop 80 80 .7
	End

	Event

		ENAME	BaseTrail
		Type	start
		At	T_ROOT

		Part1	CustomizeablePowers\DarkBlast\CreateDarknessTrail.part
		Part2	CustomizeablePowers\DarkBlast\CreateDarknessTrail2.part
		Part3	CustomizeablePowers\DarkBlast\CreateDarknessPuddleFlat.part
		Part4	CustomizeablePowers\DarkBlast\CreateDarknessPuddle2Flat.part
		Part5	CustomizeablePowers\DarkBlast\DarkTarRing2.Part
		Sound miasma_loop 80 80 .6
	End

	Event
		EName	ringBurst
		Type	start #Start
		At	T_ROOT

		Part4	CustomizeablePowers\DarkBlast\ThePitRing.Part
	End
End

###############################################################################

End