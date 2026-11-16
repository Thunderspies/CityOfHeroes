#############################################################
## Cond Control - Chillblain
#############################################################

FxInfo
Lifespan 140

#############################################################


Condition
	On 	Time
	Time 	5

	Event
		EName 	DirtUp
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset 0 0.2 0
		End
		ChildFX	:EarthEmote_CHILD.fx
		Part	:InitRockScatter.part
		Lifespan 30
	End

	Event
		Type	Local
		At	Root
		Part	:InitRockScatter.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	30

	Event
		Ename Flash
		Type	Local
		At	Hips
		Part	:Blast.part
	End
End

Condition
	On	Time
	Time	35
	Repeat	8

	Event
		Ename	Kaboom
		Type	StartPositOnly
		At	Hips
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			StartJitter		2 4 2
			Scale			0.2 0.3 0.2
			Spin 			0.2 0.1 0.1
			InitialVelocity 	0.0 1.2 0.0
			InitialVelocityJitter 	1.5 1 1.5

			FadeOutLength		45

			Drag	0.0

			physics 		1
			physGravity 		3
			physRestitution 	0.1
			physSFriction 	 	5
			physDFriction 	 	0.9
		End

		Geom	Cliff_Chunk_03
		Geom	Cliff_Chunk_04
		Geom	Cliff_Chunk_05
		Part	:RocksBigScatter.part

		CEvent	:CHILD_RockBreak.fx
		Lifespan 90

	End

	Event
		Type	StartPositOnly
		At	Root
		PArt	:Dirt_Stream_Out.part
	End

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset 0 5 0
		End
		part	:Dust_Lingering.part
	End
End


Condition
	On 	Time
	Time 	40

	Event
		EName 	DirtAfter
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 0 5 0
		End
		part	:RocksScatter.part
		part	:DirtExplosion.part
		Lifespan 5
	End
End


Condition
	On	Time
	Time	45

	Event
		Ename	PHYSICS-ExplosiveForce-
		Type	Start
		At	Chest

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		600
			PhysForcePowerJitter	75
		End
		Lifespan	1

	End
End

#############################################################

End