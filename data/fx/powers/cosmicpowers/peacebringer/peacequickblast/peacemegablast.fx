#############################################################
## FX System Name
#############################################################

FxInfo

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

	On	Time
	Time	0

	Event
		Type	Local
		At	Mystic
		Sound PeaceMega 80 80 .96
	End
End

Condition
	On 	Time
	Time 	18
	
	Event
		EName	HandGeometry
		Type	local
		At	BendMystic
		Geom	PeaceTube1
		BHVR	:GeomEnergyBlast.bhvr
		LifeSpan	25
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName	HandGeometry2
		Type	local
		At	BendMystic
		Geom	PeaceTube2
		BHVR	:GeomEnergyBlast2.bhvr
		LifeSpan	20
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	HandGeometrymotion1
		Type	local
		At	BendMystic
		BHVR	:GeomEnergySpinUP.bhvr
		Geom	RightHalfTubeDown
		LifeSpan 4
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	HandGeometrymotion2
		Type	local
		At	BendMystic
		BHVR	:GeomEnergySpin2.bhvr
		Geom	LeftTubeDown
		LifeSpan 4
	End
End

Condition
	On 	Time
	Time 	29

	Event
		EName	HandGeometrymotion1
		Type	local
		At	BendMystic
		BHVR	:GeomEnergySpin.bhvr
		Geom	RightHalfTubeDown2
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	34

	Event
		EName	HandGeometrymotion2
		Type	local
		At	BendMystic
		BHVR	:GeomEnergySpin.bhvr
		Geom	LeftTubeDown2
		LifeSpan 14
	End
End

Condition
	On 	Time
	Time 	29
	
	Event
		EName	HandGeometry
		Type	local
		At	BendMystic
		Geom	RightHalfTubeDown
		BHVR	:GeomEnergySpinUP.bhvr
		LifeSpan	4
	End

	Event
		EName	HandGeometry2
		Type	local
		At	BendMystic
		Geom	LeftTubeDown
		BHVR	:GeomEnergySpin2.bhvr
		LifeSpan	4
	End
End

Condition
	On 	Time
	Time 	18

	Event
		EName	HandGeometrymotion1
		Type	local
		At	BendMystic
		BHVR	:BlastPieceScaler.bhvr
		Geom	RightHalfTubeDown
		LifeSpan 7
	End

	Event
		EName	HandGeometrymotion2
		Type	local
		At	BendMystic
		BHVR	:BlastPieceScaler.bhvr
		Geom	LeftTubeDown
		LifeSpan 7
	End

	Event
		EName	HandGeometrymotion1
		Type	local
		At	BendMystic
		BHVR	:BlastPieceScaler2.bhvr
		Geom	RightHalfTubeDown
		LifeSpan 17
	End

	Event
		EName	HandGeometrymotion2
		Type	local
		At	BendMystic
		BHVR	:BlastPieceScaler2.bhvr
		Geom	LeftTubeDown
		LifeSpan 17
	End
End

Condition
	On 	Time
	Time 	20
	
	Event
		EName	HandGeometry
		Type	local
		At	BendMystic
		Geom	top34Tube
		BHVR	:GeomEnergyBlast4.bhvr
		LifeSpan	1
	End

	Event
		EName	HandGeometry2
		Type	local
		At	BendMystic
		Geom	Bottem34Tube
		BHVR	:GeomEnergyBlast4.bhvr
		LifeSpan	1
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	HandGeometry
		Type	local
		At	BendMystic
		Geom	top34Tube2
		BHVR	:GeomEnergyBlast3.bhvr
		LifeSpan	1
	End

	Event
		EName	HandGeometry2
		Type	local
		At	BendMystic
		Geom	Bottem34Tube2
		BHVR	:GeomEnergyBlast3.bhvr
		LifeSpan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		Type	Local
		At	wepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan	10
	End

	Event
		Type	Local
		At	wepL
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan	10
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName 	Prime
		Type	start 
		At	BendMystic
		Bhvr	:QuickBlastProjectile.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
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
		Part	:HeadElectrons.part
		Part	:HeadElectronsYellow.part
		#Part	:BlueElectricity.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime2
		Type	start 
		At	BendMystic
		Bhvr	:EnergyProjectiles1.bhvr
		Magnet	Target
		LookAt  Target
	End

	Event
		EName 	s1
		Type	local 
		At	Prime2
		Bhvr	:ProjectilesRotate1.bhvr
	End

	Event
		EName 	s2
		Type	local 
		At	s1
		Bhvr	:ProjectilesRotate1.bhvr
		Part	:OutsideBlastTrail.part
	End

	Event
		EName 	Prime3
		Type	start 
		At	BendMystic
		Bhvr	:EnergyProjectiles.bhvr
		Magnet	Target
		LookAt  Target
	End

	Event
		EName 	s3
		Type	local 
		At	Prime3
		Bhvr	:ProjectilesRotate.bhvr
	End

	Event
		EName 	s4
		Type	local 
		At	s3
		Bhvr	:ProjectilesRotate.bhvr
		Part	:OutsideBlastTrail.part
	End

End

#############################################################

Condition
	On	PrimeHit
			
	Event
		EName	AOEHit
		Type	Local
		At	T_root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
		Part1	:BlastRing.part
		Part1	:BlastThinRing.part
		LifeSpan	4
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

	Event
		Ename	All
		Type	Destroy
	End
End

#############################################################

End