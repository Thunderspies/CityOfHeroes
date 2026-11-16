#########################################################
##	Fire_Ball
##
FxInfo

#LifeSpan	200

######################################################################

Condition
	On 	Time
	Time 	27

	Event
		Type	Local
		At	head
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Sound PeaceMega 80 80 .96
		LifeSpan 10
	End

End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Prime
		Type	start 
		At	head
		Bhvr	:QuickBlastProjectile.bhvr
		Magnet	Target
		LookAt	Target
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

		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		150
			PhysForcePowerJitter	50
		End
	End

	Event
		EName 	Prime2
		Type	start 
		At	head
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

################################################

	Event
		EName 	Prime3
		Type	start 
		At	head
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

Condition
	On	PrimeHit
			
	Event
		EName	AOEHit
		Type	Local
		At	T_root
		BHVR	Behaviors/RootToChestAdjustment.bhvr

		Part1	:BlastRing.part
		Part1	:BlastThinRing.part
		LifeSpan 4
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

End	


			