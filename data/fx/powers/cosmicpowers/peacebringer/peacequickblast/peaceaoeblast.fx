#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	45
	End
End

#############################################################

Condition
	On 	Time
	Time 	17
	
	Event
		EName	HandGeometry
		Type	Local
		At	BendMystic
		Geom	PeaceTube1
		BHVR	:GeomEnergyBlast.bhvr
		LifeSpan	15
	End

	Event
		EName	HandGeometry2
		Type	Local
		At	BendMystic
		Geom	PeaceTube2
		BHVR	:GeomEnergyBlast2.bhvr
		LifeSpan	10
	End
End

Condition
	On 	Time
	Time 	13

	Event
		EName	HandGeometrymotion1
		Type	Local
		At	BendMystic
		BHVR	:GeomEnergySpinUP.bhvr
		Geom	RightHalfTubeDown
		Sound peaceAOEblast 80 80 .97
		LifeSpan	4
	End
End

Condition
	On 	Time
	Time 	13

	Event
		EName	HandGeometrymotion2
		Type	Local
		At	BendMystic
		BHVR	:GeomEnergySpin2.bhvr
		Geom	LeftTubeDown
		LifeSpan	4
	End
End

Condition
	On 	Time
	Time 	19

	Event
		EName	HandGeometrymotion1
		Type	Local
		At	BendMystic
		BHVR	:GeomEnergySpin.bhvr
		Geom	RightHalfTubeDown2
		LifeSpan	15
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName	HandGeometrymotion2
		Type	Local
		At	BendMystic
		BHVR	:GeomEnergySpin.bhvr
		Geom	LeftTubeDown2
		LifeSpan	10
	End
End

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan 20
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	start 
		At	mystic
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
		Lifespan	1
	End
End

#############################################################

End			