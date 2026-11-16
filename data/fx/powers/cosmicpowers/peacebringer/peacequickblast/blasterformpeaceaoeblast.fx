#############################################################
## BlasterFormPeaceAOEBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	16

	Event
		EName	SwordFlash2
		Type	Local
		At	head
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Sound peaceAOEblast 80 80 .97
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	start
		At	head
		Bhvr	:QuickBlastProjectile.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Part	:QuickBlastHeadCore.part
		Part	:QuickBlastCore.part
		Part	:QuickBlastCore.part
		Part	:QuickBlastCore.part
		Part	:QuickBlastPeaceEnergy2.part
		Part	:QuickBlastPeaceEnergy.part
		Part	:PeaceBlastSparks.part
		Part	:QuickBlastHeadStreaks.part
		Part	:QuickBlastHeadGlow.part
		Part	:QuickBlastHeadRing.part
		Part	:SolidQuickBlastCore.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName 	Prime
		type	Destroy
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		450
			PhysForcePowerJitter	50
		End
		Lifespan 1

	End
End

#############################################################

End