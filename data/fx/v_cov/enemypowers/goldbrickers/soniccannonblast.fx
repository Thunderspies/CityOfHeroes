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
		Type	local 
		At	head
		Sound sonicbigblast7 100 100 .99
				
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
		#LifeSpan	30
	End
	
	Event
		Type	Local 
		At	Spinner
		altpiv	1
		Geom	GoldBricker_Spinner
		BHVR	Behaviors/SonicGunSpinner.bhvr
		
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
				
	End

	Event
		EName 	rigging
		Type	local 
		At	riggingoffset
		Geom	PsiTele02
				
	End
End

Condition
	On 	Time
	Time 	20
	
	Event
		EName 	MussleFlash
		Type	Start
		At	rigging
		   Altpiv   1		   
		BHVR	:rotate2.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan	39
				
	End

End

Condition
	On 	Time
	Time 	58

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
		Part	:HugeSonicRingCore.part
		Part	:HugeSonicRingGlowTrail.part
		Part	:HugeSonicRingGlow.part
		Part	:HugeSonicRingCoreTrail.part
		Part	:HugeSonicRingCoreTrailBright.part
		Part	:HugeSonicRingCoreTrailBrightGold.part
		
	End
	
End

Condition
	On 	Time
	Time 	59

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
		Part	:HugeSonicRingCore.part
		Part	:HugeSonicRingGlowTrail.part
		#Part	:HugeSonicRingGlow.part
		Part	:HugeSonicRingCoreTrail.part
		Part	:HugeSonicRingCoreTrailBright.part
		Part	:HugeSonicRingCoreTrailBrightGold.part
		
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

End	


			