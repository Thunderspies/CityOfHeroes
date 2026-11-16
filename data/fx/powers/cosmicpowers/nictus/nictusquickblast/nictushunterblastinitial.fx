#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200
########################################################

Condition
	On 	Time
	Time 	37

	Event
		Type	Local 
		At	wepR
		Sound NictusHunter 100 100 1.0

	End

End

Condition

	Event
		EName 	EnergyCore
		Type	Local 
		At	wepR
		Geom	NictusHunter

	End

End

Condition
	On 	Time
	Time 	39

	Event
		EName 	BlastGunFlash
		Type	Start 
		At	EnergyCore
		Altpiv	1
		
		Part	:NictusRifleEnergyBLK1.part
		Part	:NictusRifleEnergyBLUE1.part
		Part	:NictusRifleEnergyPURPLE1.part
		Part	:NictusRifleEnergyRED1.part
				
	End
End

Condition
	On 	Time
	Time 	41

	Event
		EName 	BlastGunFlash
		Type	Start 
		At	EnergyCore
		Altpiv	1
		
		Part	:NictusRifleEnergyBLK.part
		Part	:NictusRifleEnergyBLUE.part
		Part	:NictusRifleEnergyPURPLE.part
		Part	:NictusRifleEnergyRED.part
				
	End
End

Condition
	On 	Time
	Time 	44

	Event
		EName 	Prime
		Type	Start 
		At	EnergyCore
		Altpiv	2
		Bhvr	:SmallBlastProjectile.bhvr
		Magnet	Target
		LookAt	Target
		
		Part	:HunterSmallBlastHeadGlow.part
		Part	:RedHeadCore.part
		Part	:RedHeadCoreHead.part
		Part	:QuickBlastHeadStreaksBlue.part
		Part	:QuickBlastHeadStreaksPurple.part
		Part	:QuickBlastHeadStreaks.part
		Part	:HunterQuickBlastHeadGlow.part
		Part	:HunterQuickBlastHeadGlowPurple1.part
		Part	:HunterQuickBlastHeadGlowBlue1.part
		Part	:HunterQuickBlastHeadRing2.part
		
	End
End

Condition
	On 	Time
	Time 	44

	Event
		EName 	Prime1
		Type	Start 
		At	EnergyCore
		Altpiv	2
		Bhvr	:SmallBlastProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SmallBlastHeadBlackCore.part
		Part	:HunterSmallNictusEnergy2.part
		Part	:HunterSmallNictusEnergyPurple2.part
		Part	:HunterSmallNictusEnergyRED.part

	End
End

Condition
	On	PrimeHit
			
	Event
		Ename	Prime
		Type	Destroy
	End

End

Condition
	On	Prime1Hit
			
	Event
		Ename	Prime1
		Type	Destroy
	End

End

End	


			