#########################################################
##	chill touch hit
FxInfo


LifeSpan	80	#180

#####################################################################
##DarkArmor
########################################################

Condition
	On	Time
	Time	0

	Event
		EName	2z
		Type	Local
		At	FootR
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		Sound darkmoonbeam2 70 70 .75
		LifeSpan	40

	End

	Event
		EName	3z
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		#PMagnet LLEGL
		#Sound 	miasma_loop 60 20 .77
		LifeSpan	40



	End

	Event
		EName	4z
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		#PMagnet FootL
		LifeSpan	40



	End

	Event
		EName	5z
		Type	Local
		At	FootL
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40



	End

	Event
		EName	6z
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		#PMagnet LlegR
		LifeSpan	40


	End

	Event
		EName	7z
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		#PMagnet FootR
		LifeSpan	40

	End

	Event
		EName	9z
		Type	Local
		At	Head
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40


	End

	Event
		EName	10z
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		#PMagnet LArmL
		LifeSpan	40


	End

	Event
		EName	11z
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		#PMagnet WepL
		LifeSpan	40

	End

	Event
		EName	13z
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part2	CustomizeablePowers\DarkMelee\Smoke2_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		#PMagnet LArmR
		LifeSpan	40


	End

	Event
		EName	14z
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\DarkMelee\Smoke.part
		part2	CustomizeablePowers\DarkMelee\Smoke2_Additive.part

		Part	CustomizeablePowers\DarkMelee\SmokeHands2.part
		Part3	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		#PMagnet WepR
		LifeSpan	40

	End

End

Condition
	On	Time
	Time	15


	Event

		ENAME	Base
		Type	start
		At	root

		Part3	CustomizeablePowers\DarkMelee\ConsumptionRadiusRing.Part
		Part5	CustomizeablePowers\DarkMelee\ConsumptionRadiusRingThin.Part
		#Sound 	Blackhole4 80 30 .75
		LifeSpan	10
	End

End


End