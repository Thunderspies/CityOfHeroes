#############################################################
## SonicBlast_Attack.fx
#############################################################

FxInfo
LifeSpan	200

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	head
		Sound sonicsmallblast3 80 80 .97
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName 	rigging
		Type	local
		At	head
		Geom	PsiTele02

	End

	Event
		EName 	MussleFlashRigging
		Type	local
		At	head
		Geom	PsionicRigging

	End

End


Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	MussleFlashRigging
		altpiv	3
	#	Part	:MussleFlashRingCore.part
	#	Part	:MussleFlashRing.part

		LifeSpan	6

	End

End

Condition
	On 	Time
	Time 	28

	Event
		EName	prime
		Type	Start
		At	WepR
	#	Geom	FrontFacingLocalPivot

		BhvrOverride
			TrackRate		2.0
			PyrRotate		90.0 0.0 0.0
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		20
			PhysForcePowerJitter	10
		End


		Magnet	T_Chest
		LookAt  T_Chest

	End

	Event
		EName	Projectile
		Type	Local
		At	Prime
		BhvrOverride
			PyrRotate		90.0 0.0 0.0
		End
		Part	:SMLSonicRingCore.part
		Part	:Sonic_Trail_Pulse.part
	End

End

#########################################################################################

######################################################################################



######################################################################################

Condition
	On	PrimeHit

	Event
		EName 	Prime
		type	Destroy

	End

	Event
		EName 	P2
		type	Destroy

	End

End


End


