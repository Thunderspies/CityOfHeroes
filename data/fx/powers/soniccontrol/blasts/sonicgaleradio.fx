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
		At	Root
		Geom	PsiTele02
		Sound sonicgale 100 100 .88
				
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
		Type	local 
		At	MussleFlashRigging
		altpiv	3
		
		BhvrOverride
			PositionOffset	.8 .2 0
		End

		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan	20
				
	End

	Event
		EName 	MussleFlash
		Type	local 
		At	MussleFlashRigging
		altpiv	3
		
		BhvrOverride
			PositionOffset	-.8 .2 0
		End

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
		bhvr	:GaleProjectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		
		BhvrOverride
			PositionOffset		0 0 5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		150
			PhysForcePowerJitter	25
		End
	End

	Event
		EName	p
		Type	Local
		At	Prime
		  AltPiv 1
		Part	:SonicCoreElipse.part
		Part	:SonicElipse.part
		Part	:GaleMist.part
		#Part	:GaleStreaks.part
		
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
		bhvr	:GaleProjectile.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
	End

	Event
		EName	p1
		Type	Local
		At	Prime1
		  AltPiv 1
		Part	:SonicCoreElipse.part
		Part	:SonicElipse.part
		Part	:GaleMist.part
		#Part	:GaleStreaks.part
		
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


			