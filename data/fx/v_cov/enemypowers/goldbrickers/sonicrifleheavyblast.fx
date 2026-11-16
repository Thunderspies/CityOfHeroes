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

###########################################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	riggingoffset
		Type	local 
		At	Spinner
		altpiv	2
		Sound SonicBigBlast7 100 100 .96
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
	Time 	20

	Event
		EName 	riggingoffset
		Type	start 
		At	Spinner
		altpiv	2
		BHVR	:muzzleFlash.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		
		LifeSpan	40
		
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
	End

	Event
		EName	p
		Type	Local
		At	Prime
		  AltPiv 1
		Part	:SonicRingCoreHuge.part
		Part	:SonicRingGlowTrailHuge.part
		Part	:SonicRingGlowHuge.part
		Part	:SonicRingCoreTrailHuge.part
		Part	:SonicRingCoreTrailBrightHuge.part
		Part	:SonicRingCoreTrailBrightGoldHuge.part

		Part	:SMLSonicRingCoreTrailBright2.part
		Part	:SMLSonicRingCoreTrailBrightGold2.part

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


			