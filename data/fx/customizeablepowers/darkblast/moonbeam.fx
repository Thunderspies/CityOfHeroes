#########################################################
##	Fire_Ball
##

#########################################################

FxInfo
LifeSpan	200

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		Sound Darkmoonbeam3b 80 80 .8
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticleWhite.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part
		PMagnet	LarmR
		Lifespan 50
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticleWhite.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	WepR
		Lifespan 50
	End

End

#########################################################################################################

Condition
	On 	Time
	Time 	5


	Event
		EName 	Prime
		Type	Start
		At	WepR

		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		3.0
		End

		Magnet	Target
		LookAt	Target

		Part	CustomizeablePowers\DarkBlast\BeamRapidTenticles.part
		Part	CustomizeablePowers\DarkBlast\DarkBlastPuddleTrail2.Part
		Part	CustomizeablePowers\DarkBlast\MoonBeamLightRays.part
		Part	CustomizeablePowers\DarkBlast\MoonBeamEmber2.part
		Part	CustomizeablePowers\DarkBlast\MoonBeamEmberLarge2.part

	End

	Event
		EName 	Prime2
		Type	Start
		At	WepR

		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr

		Magnet	Target
		LookAt	Target

		Part	CustomizeablePowers\DarkBlast\BeamRapidTenticlesHead.part
		Part	CustomizeablePowers\DarkBlast\MoonBeamRapidCloud2.part
		Part	CustomizeablePowers\DarkBlast\MoonBeamRapidCloud.part
		Part	CustomizeablePowers\DarkBlast\DarkBlastTendrill01a.part
		Part	CustomizeablePowers\DarkBlast\DarkBlastTendrill03a.part

	End

	Event
		EName 	Prime3
		Type	Start
		At	WepR

		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		Part	CustomizeablePowers\DarkBlast\DarkBlastTendrill02a.part
		Part	CustomizeablePowers\DarkBlast\DarkBlastTendrill04a.part

	End


End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End

	Event
		EName 	Prime3
		Type	Destroy

	End

End

#########################################################################################################

End


