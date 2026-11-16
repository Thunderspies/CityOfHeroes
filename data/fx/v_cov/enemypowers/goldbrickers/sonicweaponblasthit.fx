#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	30

Condition

	Event
		Type	local 
		At	Chest
		Part	:Mirage.part	
		Part	:MirageSteam.part
		Bhvr	behaviors/genericparticlefade.bhvr
		Sound sonichit2 70 70 .75

	End

	Event
		Type	local 
		At	Chest
		Part	:SonicHitflash.part
		Part	:SonicHitflashStar.part

		Part	:SonicHitRing.part
		Part	:SonicHitRingThin.part

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		600
			PhysForcePowerJitter	150
		End

		Lifespan	1

	End
End

End	


			