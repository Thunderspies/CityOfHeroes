#############################################################
## FireCometBurst.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start
		At	origin
		Part	:Mediumfire_flame_A2.part
		Part	:Mediumfire_flame_Flat.part
		Lifespan 30
	End

	Event
		EName 	Prime
		Type	start
		At	origin
		ChildFX	:FireballBurnHitDecal.fx
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	origin
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		150
			PhysForcePowerJitter	25
		End
		Lifespan 1
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	4

	Event
		Type	start
		At	origin
		Bhvr	:FireBurstUP.bhvr
		Part	:FireSplash.part
		Part	:FireSplash.part
		Lifespan	90
		LifespanJitter	30
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	chest
		Part	:fireweaponexplosion.part
		Lifespan 10
	End

	Event
		hardwareOnly
		Type	start
		At	origin
		Bhvr	:FireBurstUP.bhvr
		Part	:FireSplash.part
		Part	:FireSplash.part
		Lifespan	90
		LifespanJitter	30
	End
End

#############################################################

End