#############################################################
## MoomBeam_Attack.fx
#############################################################


#########################################################

FxInfo

LifeSpan	300

#########################################################

Input
	InpName	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		Part1	:DarkSoulBeamTenticle.part
		Sound Darkmoonbeam3b 80 80 .8
		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part



		PMagnet	LarmR
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		Part1	:DarkSoulBeamTenticle.part

		Part3	CustomizeablePowers\DarkBlast\DarkHandsEmberb.part

		PMagnet	WepR
	End

End

Condition
	On	Time
	Time	0

	Event
		EName 	HandR
		Type	local
		At	WepR

		Part	:soulFlash1.part

		Part	:soulFire2.part

		Lifespan	1
	End

	Event
		EName 	HandR
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulSmoke1.part

		Lifespan	10
	End

End

#########################################################################################################

Condition
	On 	Time
	Time 	1


	Event
		EName 	Prime
		Type	Start
		At	WepR

		Bhvr	behaviors/slowprjct5.bhvr
		Magnet	Target
		LookAt	Target

		Part	POWERS/DarknessControl/BeamRapidTenticles.part
		Part	:DarkSoulBeamLightRays.part

		Part	CustomizeablePowers\DarkBlast\MoonBeamEmber2.part
		Part	CustomizeablePowers\DarkBlast\MoonBeamEmberLarge2.part
		Part	CustomizeablePowers\DarkBlast\MoonBeamRapidCloud.part
		Part	:DarkBlastTendrill01a.part
		Part	:DarkBlastTendrill03a.part

		Part	:DarkBlastTendrill02a.part
		Part	:DarkBlastTendrill04a.part


		Part	:soulSmoke1.part
		Part	:soulSmoke1.part

		Part	:soulGlow0.part

	End


End


Condition
	On 	PrimeHit

	Event
		EName 	HitFake
		Type	local
		At	target
	End

	Event
		EName 	Prime
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	50

	Event
		EName 	HitFake
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	30

	Event
		EName 	Hand1
		Type	Destroy
	End

	Event
		EName 	Hand2
		Type	Destroy
	End


End


End


