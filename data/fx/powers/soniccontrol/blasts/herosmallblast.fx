#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

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
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	6
		
	End

End

Condition
	On 	Time
	Time 	28

	Event
		EName	prime
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:SMLProjectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest

		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		20
			PhysForcePowerJitter	10
		End
	End

	Event
		EName	p2
		Type	Local
		At	Prime
		  AltPiv 1
		Part	:SMLSonicRingCore.part
		Part	:SMLSonicRingGlowTrail.part
		Part	:SMLSonicRingGlow.part
		Part	:SMLSonicRingCoreTrail.part
		Part	:SMLSonicRingCoreTrailBright.part
		Part	:SMLSonicRingCoreTrailBrightGreen.part
		
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


			