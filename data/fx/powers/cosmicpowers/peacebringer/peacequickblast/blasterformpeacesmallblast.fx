#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

######################################################################
Condition
	On 	Time
	Time 	16
	Event
		EName	SwordFlash2
		Type	Local
		At	head
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Sound peaceblast1 80 80 .8
		LifeSpan 2
	End
End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Start 
		At	head
		Bhvr	:QuickBlastProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadGlow.part
		Part	:SmallBlastSparks.part
		Part	:SmallBlastHeadStreaks.part

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		100
			PhysForcePowerJitter	50
		End

	End

End

Condition
	On	PrimeHit
	
	Event
		Ename	Prime
		Type	Destroy
	End

End

End	


			