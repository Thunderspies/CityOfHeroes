#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		
		Part	:NictusSunFlareHitStreaks.part
		Part	:BlastFlash.part
		Part	:BlastFlashTendrils.part
		Part	:QuickBlastHitStreaks.part
		Part	:QuickBlastHitStreaksBlue.part
		Part	:PeaceBlastHitSparks.part
		Part	:BlastFlashTendrilsPurple.part
		Part	:BlastFlashTendrilsBlue.part
		Sound Warshadehit1 80 80 .88

		LifeSpan 10
	End

		
	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		250
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End


End	


			