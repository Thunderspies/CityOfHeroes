#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

#########################################################

Condition
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound aim4 60 60 .6
		End
end


Condition
	On Time
	Time 0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part	POWERS/ChestLevelUpRaysThinFaint.part
		Part1	POWERS/MultiUpGlow.part
		Part2	POWERS/MultiUpSpecks2.part
		Part4	V_COV/Powers/Patron/SoulMastery/OppressiveGloom/GhostWidowSigil.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 10

	Event
		EName	Pow
		Type 	local
		At	Chest
		Part2	POWERS/MultiUpRing01.part
		Part3	POWERS/MultiUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 20

	Event
		EName	Pow
		Type 	local
		At	Chest
		Part3	POWERS/MultiUpRing02.part
		Part4	POWERS/MultiUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 30

	Event
		EName	Pow
		Type 	local
		At	Chest
		Part2	POWERS/MultiUpRingSlow.part
		Part3	POWERS/MultiUpRing03.part
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End


Condition
	On Time
	Time 13

	Event
		EName	pow2
		Type 	Destroy

	End

End

Condition
	On Time
	Time 75

	Event
		EName	all
		Type 	Destroy

	End

End




## AURA FX ######################################################


################   7   ##########################

Condition
	On	Time
	Time	5
	Event
		EName 	binder7
		Type	local
		At	handR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr

		Lifespan	45
	End

	Event
		EName 	Offset7
		Type	local
		At	binder7
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset7
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset7
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   8   ##########################

Condition
	On	Time
	Time	5

	Event
		EName 	binder8
		Type	local
		At	handL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr

		Lifespan	45
	End

	Event
		EName 	Offset8
		Type	local
		At	binder8
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset8
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset8
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End

################   9   ##########################

Condition
	On	Time
	Time	15
	Event
		EName 	binder9
		Type	local
		At	larmR
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm1.bhvr

		Lifespan	35
	End

	Event
		EName 	Offset9
		Type	local
		At	binder9
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset9
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset9
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


################   10   ##########################

Condition
	On	Time
	Time	15

	Event
		EName 	binder10
		Type	local
		At	larmL
		Bhvr	V_COV\EnemyPowers\GhostWidow\soulBinderArm2.bhvr

		Lifespan	35
	End

	Event
		EName 	Offset10
		Type	local
		At	binder10
		Bhvr	V_COV\EnemyPowers\GhostWidow\binderOffset.bhvr
		Part	V_COV\EnemyPowers\GhostWidow\soulGlow0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
		Part	V_COV\EnemyPowers\GhostWidow\soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1
		Type	local
		At	Offset10
		  AltPiv	2
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

	Event
		EName 	Arm2
		Type	local
		At	Offset10
		  AltPiv	4
		Part	V_COV\EnemyPowers\GhostWidow\soulFire0.part
	End

End


End
##########################