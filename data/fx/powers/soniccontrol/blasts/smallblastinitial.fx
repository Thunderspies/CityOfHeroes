#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200


Condition
	On 	Time
	Time 	11

	Event
		Type	Local 
		At	wepR
		Sound SonicSmallBlast2 80 80 .92
		#LifeSpan	30
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Spinner
		Type	Local 
		At	wepR
		Geom	SonicSpinnerOffset
		#Sound	SonicSmallBlast2 80 70 .92
		#LifeSpan	30
	End
	
	Event
		Type	Local 
		At	Spinner
		altpiv	1
		Geom	SonicSpinner2
		BHVR	Behaviors/SonicGunSpinner.bhvr
		
		LifeSpan	9
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
	Time 	38

	Event
		EName	prime
		Type	Start
		At	rigging
		   Altpiv   1
		Geom	FrontFacingLocalPivot
		bhvr	:SMLProjectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
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


			