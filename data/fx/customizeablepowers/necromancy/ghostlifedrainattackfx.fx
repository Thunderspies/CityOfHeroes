#########################################################
##	chill touch hit
FxInfo

lifespan	200
#########################################################

Condition

	Event
		EName	Prime
		Type	start
		At	T_Chest
		Bhvr	behaviors/slowprjctLifeDrain.bhvr
		Part	:SkullTenticlesBlackLite.part
		Part	:SkullTenticlesBlackLite.part
		Part	:SkullTenticlesLite.part
		Part	:SkullTenticlesLite.part
		Part	:SkullTenticlesLite.part
		Part	:Skullsmoke.part

		Magnet	WepL
		Lookat	WepL

	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	:GreenHandTenticle.part
		Part3	:GreenHandsEmber.part
		Part	:GreenHandTenticleAdditive.part

		PMagnet	LarmL
		lifespan	20
	End

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	:GreenHandTenticle.part
		Part3	:GreenHandsEmber.part
		Part	:GreenHandTenticleAdditive.part

		PMagnet	WepL
		lifespan	20
	End

End

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy

	End

	Event
		Type	local
		At	WepL
		Sound heal1 70 70 .88
		Magnet	WepL
		Lookat	WepL
		LifeSpan	23

	End

	Event
		EName 	HandGlow
		Type	Local
		At	WepL
		Part1	:GreenHandGlow.part
		lifespan	25
	End

	Event
		Type	Local
		At	back
		BHVR	Behaviors/GhostPivot.bhvr
		Part	:SoulGlo.part

		lifeSpan	20
	End

	Event
		Type	Local
		At	back
		BHVR	Behaviors/GhostPivot.bhvr

		Part	:SoulFlame2.part

		lifeSpan	20

	End

	Event
		Type	Local
		At	back
		BHVR	Behaviors/GhostPivot.bhvr

		Part	:SoulFlame.part

		lifeSpan	20
	End


End



End


