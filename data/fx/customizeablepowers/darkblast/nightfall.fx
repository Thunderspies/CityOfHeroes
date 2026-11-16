#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	180



###########################################################


Condition
	On	Time
	Time	0

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		Sound nightFall2 100.0 100.0 .85

		PMagnet	LarmR
		LifeSpan	50
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	LarmL
		LifeSpan	50
	End


End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	WepR
		LifeSpan	50
	End



	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	WepL
		LifeSpan	50
	End

End

Condition
	On	Time
	Time	30


	Event
		EName	Prime
		Type	start
		At	WepL
		Part1	CustomizeablePowers\DarkBlast\DarknessRapidTenticles.part
		Part4	CustomizeablePowers\DarkBlast\DarknessRapidTenticlesCore.part
		Part2	CustomizeablePowers\DarkBlast\DarknessRapidCloud.part
		Part3	CustomizeablePowers\DarkBlast\DarknessRapidCloud2.part
		#Part5	:SmallTenticlesHead.part

		bhvr	CustomizeablePowers\DarkBlast\projectile1.bhvr
		Magnet	Target
		LookAt  Target

	End


End


Condition
	On	Time
	Time	37


	Event
		EName	Prime1
		Type	start
		At	WepR

		Part1	CustomizeablePowers\DarkBlast\DarknessRapidTenticles.part
		Part4	CustomizeablePowers\DarkBlast\DarknessRapidTenticlesCore.part
		Part2	CustomizeablePowers\DarkBlast\DarknessRapidCloud.part
		Part3	CustomizeablePowers\DarkBlast\DarknessRapidCloud2.part
		#Part5	:SmallTenticlesHead.part

		bhvr	CustomizeablePowers\DarkBlast\projectile1.bhvr
		Magnet	Target
		LookAt  Target

	End

End

Condition
	On	Time
	Time	57


	Event
		EName	Prime2
		Type	start
		At	WepL

		Part1	CustomizeablePowers\DarkBlast\DarknessRapidTenticles.part
		Part4	CustomizeablePowers\DarkBlast\DarknessRapidTenticlesCore.part
		Part2	CustomizeablePowers\DarkBlast\DarknessRapidCloud.part
		Part3	CustomizeablePowers\DarkBlast\DarknessRapidCloud2.part
		#Part5	:SmallTenticlesHead.part

		bhvr	CustomizeablePowers\DarkBlast\projectile1.bhvr
		Magnet	Target
		LookAt  Target

	End


End

Condition
	On	Time
	Time	49

	Event
		EName	Prime3
		Type	start
		At	WepR

		Part1	CustomizeablePowers\DarkBlast\DarknessRapidTenticles.part
		Part4	CustomizeablePowers\DarkBlast\DarknessRapidTenticlesCore.part
		Part2	CustomizeablePowers\DarkBlast\DarknessRapidCloud.part
		Part3	CustomizeablePowers\DarkBlast\DarknessRapidCloud2.part
		#Part5	:SmallTenticlesHead.part

		bhvr	CustomizeablePowers\DarkBlast\projectile1.bhvr
		Magnet	Target
		LookAt  Target

	End


End

Condition
	On	Time
	Time	44

	Event
		EName	Prime4
		Type	start
		At	WepL

		Part1	CustomizeablePowers\DarkBlast\DarknessRapidTenticles.part
		Part4	CustomizeablePowers\DarkBlast\DarknessRapidTenticlesCore.part
		Part2	CustomizeablePowers\DarkBlast\DarknessRapidCloud.part
		Part3	CustomizeablePowers\DarkBlast\DarknessRapidCloud2.part
		#Part5	:SmallTenticlesHead.part

		bhvr	CustomizeablePowers\DarkBlast\projectile1.bhvr
		Magnet	Target
		LookAt  Target

	End


End

Condition
	On 	Time
	Time	40

	Event
		EName 	HandGlowRight1
		Type	Destroy


	End

	Event
		EName 	HandGlowLeft1
		Type	Destroy


	End

End

Condition
	On 	Time
	Time	60

	Event
		EName 	HandGlowRight2
		Type	Destroy


	End

	Event
		EName 	HandGlowLeft2
		Type	Destroy


	End

	Event
		EName 	HandGlowRight22
		Type	Destroy


	End

	Event
		EName 	HandGlowLeft22
		Type	Destroy


	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy


	End

End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy


	End

End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy


	End

End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy


	End


End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy


	End

End


End