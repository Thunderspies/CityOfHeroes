#############################################################
## Consumption_Subtractive.fx
#############################################################

FxInfo
LifeSpan	80

#############################################################

Input
	Inpname	Hips
End

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
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		Sound darkmoonbeam2 70 70 .75
		LifeSpan	40

	End

	Event
		EName	3z
		Type	Local
		At	ULEGL
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40



	End

	Event
		EName	4z
		Type	Local
		At	LLEGL
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40



	End

	Event
		EName	5z
		Type	Local
		At	FootL
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40



	End

	Event
		EName	6z
		Type	Local
		At	ULEGR
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40


	End

	Event
		EName	7z
		Type	Local
		At	LLEGR
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40

	End

	Event
		EName	9z
		Type	Local
		At	Head
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40


	End

	Event
		EName	10z
		Type	Local
		At	UArmL
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40


	End

	Event
		EName	11z
		Type	Local
		At	LArmL
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40

	End

	Event
		EName	13z
		Type	Local
		At	UArmR
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
		LifeSpan	40


	End

	Event
		EName	14z
		Type	Local
		At	LArmR
		part	CustomizeablePowers\DarkMelee\Smoke.part
		part	CustomizeablePowers\DarkMelee\Smoke2.part
		Part	CustomizeablePowers\DarkMelee\SmokeHands2_Subtractive.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril_Subtractive2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril.part
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

		Part	CustomizeablePowers\DarkMelee\ConsumptionRadiusRing.Part
		Part	CustomizeablePowers\DarkMelee\ConsumptionRadiusRingThin.Part
		LifeSpan	10
	End

End


End