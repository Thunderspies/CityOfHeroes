#########################################################
##	onfire
##
FxInfo  

LifeSpan 100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start 
		At	origin
		Part1	explosions/large/Mediumfire_flame_A2.part
#		Part2	explosions/large/Mediumfire_flame_B.part
		
		Part3	explosions/large/Mediumfire_flame_Flat.part
		#Part4	WORLD/Fire/spark1.part
#		Part5	explosions/large/mediumfire_Smoke.part
		#Sound	torch2_loop 35 20 .33
		Lifespan 30
	End

	Event
		EName 	Prime
		Type	start 
		At	origin
		ChildFX 	V_COV\World\Vault\FireballBurnHitDecal.fx
	End
	
	Event
		EName 	Prime
		Type	start 
		At	origin
		ChildFX 	:FireNapalmHit.fx
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

		Lifespan	1

	End
End


Condition
	On 	Time
	Time 	0

	Repeat	4

	Event
		Type	start
		At	origin

		Bhvr	:FireBurstUP.bhvr
		Part1	:FireSplash.part
		Part5	:FireSplash.part
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

		Part1	\POWERS\FireControl\fireweaponexplosion.part
		Lifespan	10
	End

	Event
		hardwareOnly
		Type	start
		At	origin

		Bhvr	:FireBurstUP.bhvr
		Part1	:FireSplash.part
		Part5	:FireSplash.part
		Lifespan	90
		LifespanJitter	30
		
	End
End

End			
