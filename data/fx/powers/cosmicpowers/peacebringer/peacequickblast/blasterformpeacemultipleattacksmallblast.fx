#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200
#################################################################

Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	head
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Sound PeaceMulti2 90 90 1.0
		LifeSpan 5
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	T_head
		Bhvr	:QuickBlastProjectile4.bhvr
		Magnet	Chest
		LookAt	Chest
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadGlow.part
		Part	:SmallBlastSparks.part
		Part	:SmallBlastHeadStreaks.part

	End

End

Condition
	On	PrimeHit
	
	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		Part	:BlastFlash.part
		Part	:BlastFlashTendrils.part
		Part	:QuickBlastHitStreaks.part
		Part	:PeaceBlastHitSparks.part
		Part	:SunFlareHitStreaks.part
		#Part	:LightFlareHitStreaks.part
		#Part	:PeaceBlastHitSpinners.part
		Sound Peacehit1 80 80 .8
		LifeSpan 10
	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		250
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End

End	


			