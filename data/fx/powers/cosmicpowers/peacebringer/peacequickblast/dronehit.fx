#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	20

Condition
	On 	Time
	Time 	15
	Event
		EName	SwordFlash2
		Type	start
		At	mystic
		#Bhvr	:DroneHitOffset.bhvr
		Part	:DroneHitFlash.part
		Part	:DroneHitTendrils.part
		Part	:DroneHitTendrils2.part
		Part	:DroneHitStreaks.part
		Part	:DroneHitSparks.part
		LifeSpan 10
	End

	Event
		Type	start
		At	mystic		
		
		Part	:DroneHitSparksHuge.part
		Part	:DroneHitSparksHuge.part

		LifeSpan 15
	End

	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End
End


End	


			