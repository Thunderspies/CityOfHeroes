#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName 	rigging
		Type	local 
		At	head
		Geom	PsiTele02
				
	End
End

Condition
	On 	Time
	Time 	30
	
	Event
		EName 	MussleFlash
		Type	start 
		At	rigging
		   Altpiv   1
		BHVR	:rotate2.bhvr
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan	10
				
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


			