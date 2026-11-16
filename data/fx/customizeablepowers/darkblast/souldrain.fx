

#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part
		Sound SoulDrain2 80 80 .86

		PMagnet	LarmL
		LifeSpan	50
	End
End

Condition
	On 	Time
	Time 	14

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	WepL
		LifeSpan	42
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	Prime
		Type	start
		At	WepL

		Geom	Skull
		Bhvr	CustomizeablePowers\DarkBlast\Skullprojectile1.bhvr

		Magnet	Target
		LookAt  Target


	End

	Event
		EName	eye1
		Type	local
		At	Prime
		 altpiv	2
		Part1	CustomizeablePowers\DarkBlast\DaemonEyes2.part
		Part2	CustomizeablePowers\DarkBlast\DaemonPupil2.part
	End

	Event
		EName	eye2
		Type	local
		At	Prime
		 altpiv	3
		Part1	CustomizeablePowers\DarkBlast\DaemonEyes2.part
		Part2	CustomizeablePowers\DarkBlast\DaemonPupil2.part
	End

	Event
		EName	Prime2
		Type	start
		At	Prime
		 altpiv	1

		Part1	CustomizeablePowers\DarkBlast\DarknessRapidTenticles.part
		Part2	CustomizeablePowers\DarkBlast\DarknessRapidCloud.part
		Part3	CustomizeablePowers\DarkBlast\DarknessRapidCloud2.part
		Part4	CustomizeablePowers\DarkBlast\SoulDrainSkullGlow.part

		Bhvr	CustomizeablePowers\DarkBlast\Skullprojectile1.bhvr

		Magnet	Target
		LookAt  Target

	End

End

Condition
	On 	PrimeHit

	Event
		EName	skullHitFake
		Type	local
		At	T_Chest

		Part1	CustomizeablePowers\DarkBlast\SkullHit.part
		Part2	CustomizeablePowers\DarkBlast\SkullPuddle.Part
		LifeSpan	25
	End

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End
End


End



