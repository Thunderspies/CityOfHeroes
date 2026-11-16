#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

###########################################################################################################

Condition
	On 	Time
	Time 	6

	Event
		Type	local 
		At	head
		Sound SonicScream1 80 80 .99
				
	End
End




Condition
	On 	Time
	Time 	0

	Event
		EName 	rigging
		Type	local 
		At	head
		Geom	PsionicRigging

		BhvrOverride
			PYRRotate	0 0 0
		End		
				
	End

	Event
		EName 	FrontSpeakerLeft
		Type	local 
		At	head
		Geom	PsionicRigging	

		BhvrOverride
			PYRRotate	-45 -60 0
			PositionOffset	-.7 0 .9
		End
				
	End

	Event
		EName 	FrontSpeakerRight
		Type	local 
		At	head
		Geom	PsionicRigging	

		BhvrOverride
			PYRRotate	-40 60 0
			PositionOffset	1.1 0 .6
		End
				
				
	End

	Event
		EName 	RearSpeakerLeft
		Type	local 
		At	head
		Geom	PsionicRigging	
			
		BhvrOverride
			PYRRotate	30 110 -110
			PositionOffset	.6 1.6 .2
		End	
	End

	Event
		EName 	RearSpeakerRight
		Type	local 
		At	head
		Geom	PsionicRigging	
			
		BhvrOverride
			PYRRotate	30 220 40
			PositionOffset	-1 1.2 .3
		End	
	End

	
End

Condition
	On 	Time
	Time 	17

	Event
		Type	start 
		At	rigging
		altpiv	3
		Part	:MussleFlashRingCoreSmall.part
		Part	:MussleFlashRingSmall.part
		LifeSpan	20
				
	End

	Event
		Type	start 
		At	FrontSpeakerLeft
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	30
		
	End

	Event
		Type	start 
		At	FrontSpeakerRight
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	30
		
	End

	Event
		Type	start 
		At	RearSpeakerLeft
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	30
		
	End

	Event
		Type	start 
		At	RearSpeakerRight
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	30
		
	End
End


Condition
	On 	Time
	Time 	25

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
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
	End

	Event
		EName	p2
		Type	Local
		At	Prime
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

#########################################################################################

######################################################################################

Condition
	On 	Time
	Time 	20

	Event
		EName	prime1
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
	End

	Event
		EName	p1
		Type	Local
		At	Prime1
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
	On 	Time
	Time 	30

	Event
		EName	prime2
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
	End

	Event
		EName	p2
		Type	Local
		At	Prime2
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
	On 	Time
	Time 	40

	Event
		EName	prime3
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
	End

	Event
		EName	p3
		Type	Local
		At	Prime3
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
	On 	Time
	Time 	50

	Event
		EName	prime4
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		20
			PhysForcePower		15
			PhysForcePowerJitter	5
		End
	End

	Event
		EName	p4
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


Condition
	On	Prime1Hit
		
	Event
		EName 	Prime1
		type	Destroy
	
	End

	Event
		EName 	P1
		type	Destroy
	
	End

End

Condition
	On	Prime2Hit
		
	Event
		EName 	Prime2
		type	Destroy
	
	End

	Event
		EName 	P2
		type	Destroy
	
	End

End

Condition
	On	Prime3Hit
		
	Event
		EName 	Prime3
		type	Destroy
	
	End

	Event
		EName 	P3
		type	Destroy
	
	End

End

Condition
	On	Prime4Hit
		
	Event
		EName 	Prime4
		type	Destroy
	
	End

	Event
		EName 	P4
		type	Destroy
	
	End

End

End	


			