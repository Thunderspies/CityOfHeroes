#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName	Spinner
		Type	Local 
		At	wepR
		Geom	SonicSpinnerOffset02
		#LifeSpan	30
		
	End
	
	Event
		Type	Local 
		At	Spinner
		altpiv	1
		Geom	Goldbricker_Spinner_02
		BHVR	:SonicGunSpinner.bhvr
		
		LifeSpan	30
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
	Time 	37

	Event
		EName 	riggingoffset
		Type	local 
		At	Spinner
		altpiv	2
		BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		Sound SonicBlastRifle 90 90 .89
		LifeSpan	3
		
	End

End

Condition
	On 	Time
	Time 	39

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
		Part	:SMLSonicRingCoreTrailBrightGold.part
		
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


			