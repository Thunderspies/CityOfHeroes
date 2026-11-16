#########################################################
##	Fire_Ball
##

#########################################################

FxInfo
LifeSpan	200

#########################################################

Condition
	On 	Time
	Time 	11

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	LarmR
		Sound dctorrent 90 90 .85
		LifeSpan	25
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	LarmL
		LifeSpan	25
	End

	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	WepR
		LifeSpan	25
	End



	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	CustomizeablePowers\DarkBlast\DarkBlastTenticle.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb_Alpha.part

		PMagnet	WepL
		LifeSpan	25
	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	p
		Type	Start
		At	Root
		Geom	Torrent
	End

	Event
		EName 	Prime
		Type	Start
		At	p
		 Altpiv	1
		Bhvr	CustomizeablePowers\DarkBlast\MedianProjectile4.bhvr

		Part5	CustomizeablePowers\DarkBlast\TendrilTenticlesColor.part
		Part1	CustomizeablePowers\DarkBlast\TendrilTenticles.part

		Part2	CustomizeablePowers\DarkBlast\TendrillClouds2.part
		Part3	CustomizeablePowers\DarkBlast\TendrillClouds.part

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

End


End


