#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

###########################################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Spinner
		Type	Local 
		At	wepR
		Geom	SonicSpinnerOffset
		#LifeSpan	30
	End
	
	Event
		Type	Local 
		At	Spinner
		altpiv	1
		Geom	SonicSpinner2
		BHVR	Behaviors/SonicGunSpinner.bhvr
		
		LifeSpan	30
	End


End

###########################################################################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local 
		At	Spinner
		Sound SonicBigBlast7 100 100 .96
				
	End
	
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	riggingoffset
		Type	local 
		At	Spinner
		altpiv	2
		#Sound	SonicBigBlast7 100 99 .96
				
	End

	Event
		EName 	rigging
		Type	local 
		At	riggingoffset
		BHVR	:rotate.bhvr
		Geom	PsiTele02
				
	End
End


Condition
	On 	Time
	Time 	40

	Event
		EName 	riggingoffset
		Type	start 
		At	Spinner
		altpiv	2
		BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	6
		
	End

End

Condition
	On 	Time
	Time 	60

	Event
		EName	prime
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest

			
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		30
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
	End

	Event
		EName	p
		Type	Local
		At	Prime4
		  AltPiv 1
		Part	:SonicRingCore.part
		Part	:SonicRingGlowTrail.part
		Part	:SonicRingGlow.part
		Part	:SonicRingCoreTrail.part
		Part	:SonicRingCoreTrailBright.part
		Part	:SonicRingCoreTrailBrightGreen.part

		Part	:SMLSonicRingCoreTrailBright2.part
		Part	:SMLSonicRingCoreTrailBrightGreen2.part
	End
	
End

Condition
	On	PrimeHit
		
	Event
		EName 	Prime
		type	Destroy
	
	End

	Event
		EName 	P
		type	Destroy
	
	End

End


End	
			