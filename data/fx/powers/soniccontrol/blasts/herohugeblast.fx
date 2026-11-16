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
		EName 	rigging
		Type	local 
		At	head
		Geom	PsiTele02
		Sound sonic1 80 80 .77
				
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
	Time 	23
	
	Event
		EName 	MussleFlash
		Type	start 
		At	MussleFlashRigging
		altpiv	3
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan	20
				
	End

End

Condition
	On 	Time
	Time 	33

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
		Part	:HugeSonicRingCore.part
		Part	:HugeSonicRingGlowTrail.part
		Part	:HugeSonicRingGlow.part
		Part	:HugeSonicRingCoreTrail.part
		Part	:HugeSonicRingCoreTrailBright.part
		Part	:HugeSonicRingCoreTrailBrightGreen.part
		
	End
	
End

Condition
	On 	Time
	Time 	36

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
		Part	:HugeSonicRingCoreTrailBrightGreen.part
		
	End
	
End

Condition
	On 	Time
	Time 	39

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
		Part	:HugeSonicRingCore.part
		Part	:HugeSonicRingGlowTrail.part
		#Part	:HugeSonicRingGlow.part
		Part	:HugeSonicRingCoreTrail.part
		Part	:HugeSonicRingCoreTrailBright.part
		Part	:HugeSonicRingCoreTrailBrightGreen.part
		
	End
	
End

#
#Condition
#	On 	Time
#	Time 	43
#
#	Event
#		EName	prime3
#		Type	Start
#		At	rigging
#		   Altpiv   1
#		Geom	FrontFacingLocalPivot
#		bhvr	:Projectile.bhvr
#		Magnet	T_Chest
#		LookAt  T_Chest
#	End
#
#	Event
#		EName	p3
#		Type	Local
#		At	Prime3
#		  AltPiv 1
#		Part	:HugeSonicRingCore.part
#		Part	:HugeSonicRingGlowTrail.part
#		#Part	:HugeSonicRingGlow.part
#		Part	:HugeSonicRingCoreTrail.part
#		Part	:HugeSonicRingCoreTrailBright.part
#		Part	:HugeSonicRingCoreTrailBrightGreen.part
#		
#	End
#	
#End

#########################################################################################

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

End	


			