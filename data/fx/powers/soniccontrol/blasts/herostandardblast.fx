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
		Sound HeroSonicBlast2 80 80 .99
				
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
		
		LifeSpan	36
		
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
	Time 	40

	Event
		EName	prime1
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
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
	Time 	50

	Event
		EName	prime2
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
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
	Time 	60

	Event
		EName	prime3
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
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
	Time 	70

	Event
		EName	prime4
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:Projectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
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


			