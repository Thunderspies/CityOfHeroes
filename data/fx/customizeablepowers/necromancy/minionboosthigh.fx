#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

#########################################################


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
		LifeSpan	30

	End

	Event

		ENAME	Base
		Type	local
		At	WepL

		Part1	CustomizeablePowers\Necromancy\DarkHand.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmber.part
		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		part1	CustomizeablePowers\Necromancy\SmokeHands.part
		part2	CustomizeablePowers\Necromancy\SmokeHands2.part
		Part3	CustomizeablePowers\Necromancy\DarkBlastHitTendril.part
		Sound miasma3 100 100 .8

		PMagnet	LarmR
		LifeSpan	30
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		part1	CustomizeablePowers\Necromancy\SmokeHands.part
		part2	CustomizeablePowers\Necromancy\SmokeHands2.part
		Part3	CustomizeablePowers\Necromancy\DarkBlastHitTendril.part

		PMagnet	LarmL
		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		part1	CustomizeablePowers\Necromancy\SmokeHands.part
		part2	CustomizeablePowers\Necromancy\SmokeHands2.part
		Part3	CustomizeablePowers\Necromancy\DarkBlastHitTendril.part

		PMagnet	WepR
		LifeSpan	30
	End



	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		part1	CustomizeablePowers\Necromancy\SmokeHands.part
		part2	CustomizeablePowers\Necromancy\SmokeHands2.part
		Part3	CustomizeablePowers\Necromancy\DarkBlastHitTendril.part

		PMagnet	WepL
		LifeSpan	30
	End

End


Condition
	On 	Time
	Time 	14


	Event
		EName 	Prime
		Type	Start
		At	Root
		Bhvr	CustomizeablePowers\Necromancy\slowprjct5.bhvr

		part	CustomizeablePowers\Necromancy\FlatEvilSmoke.part
		part	CustomizeablePowers\Necromancy\FlatEvilSmoke2.part
		part	CustomizeablePowers\Necromancy\EvilSmoke.part
		part	CustomizeablePowers\Necromancy\EvilSmoke2.part
		Part	CustomizeablePowers\Necromancy\EvilTendril.part
		Part	CustomizeablePowers\Necromancy\Roaches.part
		Part	CustomizeablePowers\Necromancy\Beatles.part
		Part	CustomizeablePowers\Necromancy\Bees.part
		Part	CustomizeablePowers\Necromancy\Bees.part
		Part	CustomizeablePowers\Necromancy\Bees.part


		Magnet	T_Root
		LookAt	T_Root

	End

End

Condition
	On 	PrimeHit

	Event
		Type	start
		At	T_Root
		BHVR	CustomizeablePowers\Necromancy\Blacksplat.bhvr

		Splat	Ember.tga
		LifeSpan	15
	End

#################################################################################################

	Event
		Type	Local
		At	T_Root
		Part	CustomizeablePowers\Necromancy\HitEvilBeatles.part
		Part	CustomizeablePowers\Necromancy\HitEvilRoaches.part
		part	CustomizeablePowers\Necromancy\SmallDarkStreaks.part
		part	CustomizeablePowers\Necromancy\DarkGhosts.part
		part	CustomizeablePowers\Necromancy\DarkGhosts2.part
		part	CustomizeablePowers\Necromancy\DarkGhosts3.part

		LifeSpan	50
	End

	Event
		Type	Local
		At	T_Hips
		Part	CustomizeablePowers\Necromancy\BeesHit.part
		LifeSpan	70
	End

	Event
		EName 	OrbMist
		Type	Local
		At	T_Root
		geom	FX_RuneStreak
		bhvr	CustomizeablePowers\Necromancy\SpinFast.bhvr
		LifeSpan	60
	End

	Event
		EName 	OrbStreak
		Type	Local
		At	T_Root
		geom	FX_RuneStreak
		bhvr	CustomizeablePowers\Necromancy\SpinFast2.bhvr

		LifeSpan	80
	End

	Event
		EName 	Orb
		Type	Local
		At	OrbStreak
		altpiv  1
		Part	CustomizeablePowers\Necromancy\PurpleMagic.part
		part	CustomizeablePowers\Necromancy\HitEvilSmoke.part
		part	CustomizeablePowers\Necromancy\HitEvilSmoke2.part
		part	CustomizeablePowers\Necromancy\HitEvilTendril.part


	End

	Event
		EName 	Orb
		Type	Local
		At	OrbMist
		altpiv  1
		Part	CustomizeablePowers\Necromancy\PurpleMagic.part
		part	CustomizeablePowers\Necromancy\HitEvilSmoke.part
		part	CustomizeablePowers\Necromancy\HitEvilSmoke2.part
		part	CustomizeablePowers\Necromancy\HitEvilTendril.part


	End

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


End


