#########################################################
##	goldbrick sonic melee hit
########################################################
FxInfo

Input  
	Inpname	Chest
End

Lifespan 6

Condition
	On 	Time
	Time 	0

	
	Event
		EName 	Mallet
		Type	start 
		At	Chest
		
		Part2	:SonicMeleeHitStar.part
		Part3	:SonicMeleeHitRing.part
		Part4	:SonicMeleeHitRing2.part
		Sound PunchHit2 100.0 100.0 .8
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
		Type	local 
		At	Chest
		Part	:Mirage.part	
		Part	:MirageSteam.part
		Bhvr	behaviors/genericparticlefade.bhvr
		Sound sonichit2 70 70 .75

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		12
			PhysForcePower		500
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End


End
