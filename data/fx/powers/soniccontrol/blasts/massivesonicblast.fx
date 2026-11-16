#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

###########################################################################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	local 
		At	head
		Sound Sonic14 80 80 .99
				
	End
End


Condition
	On 	Time
	Time 	37

	Event
		Type	local 
		At	head
		Sound SonicMassiveBlast 100 100 .99
				
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
	Time 	37

	Event
		Type	start 
		At	MussleFlashRigging
		altpiv	3
		#BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	11
		
	End

End

Condition
	On 	Time
	Time 	45

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
		EName	p2
		Type	Local
		At	Prime
		  AltPiv 1
		Part	:MassiveSonicRingCore.part
		Part	:MassiveSonicRingGlowTrail.part
		Part	:MassiveSonicRingGlow.part
		Part	:MassiveSonicRingCoreTrail.part
		Part	:MassiveSonicRingCoreTrailBright.part
		Part	:MassiveSonicRingCoreTrailBrightGreen.part

		Part	:MassiveSonicRingCoreTrailBright2.part
		Part	:MassiveSonicRingCoreTrailBrightGreen2.part
		
	End
	
End

#########################################################################################

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


			