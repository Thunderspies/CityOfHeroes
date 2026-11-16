#
FxInfo
LifeSpan 140


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ToV_EarthSibyl_UpThrust_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type StartPositOnly
			EName DirtUp
			BhvrOverride
				PositionOffset 0 0.2 0
			End
			Part :InitRockScatter03.part
		Part	:Dirt_Stream_Down.part
		Part	:Dirt_Lingering.part
			LifeSpan 20
		End
	End

	Condition
		On Time
		Time 5

		Event
			At Root
			Type StartPositOnly
			EName DirtUp
			BhvrOverride
				PositionOffset 0 0.2 0
			End
			#Part :InitRockScatter02.part
			Part :InitRockScatter04.part
			LifeSpan 10
		End
	End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type StartPositOnly
			ChildFx :Earth_UpThrust_CHILD.fx
			EName DirtUp
			BhvrOverride
				PositionOffset 0 0.2 0
			End
			Part :InitRockScatter02.part
			Part :InitRockScatter.part
			Splat Crater_Cracks2.tga
			LifeSpan 20
		End

		Event
			At Root
			Type StartPositOnly
			Bhvr CustomizeablePowers\SuperStrength\Splat_FootStomp.bhvr
			BhvrOverride
				Scale 50 15 50
				StartColor 73 57 39
			End
			Splat Crater_Cracks2.tga
			LifeSpan 90
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Hips
			Type Local
			EName Flash
			Part :Blast.part
		End

	End

	Condition
		On Time
		Time 35
		Repeat 12

		Event
			At Hips
			Type StartPositOnly
			CEvent :CHILD_RockBreak.fx
			EName Kaboom
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				Drag 0
				InitialVelocity 0 1.2 0
				InitialVelocityJitter 1.5 1 1.5
				Scale 0.35 0.50 0.35
				Spin 0.2 0.1 0.1
				StartJitter 6 6 6
				FadeOutLength 45
				physics 1
				physDFriction 0.9
				physGravity 3
				physRestitution 0.1
				physSFriction 5
			End
			Part :RocksBigScatter.part
			Geom Cliff_Chunk_03
			Geom Cliff_Chunk_04
			Geom Cliff_Chunk_05
			LifeSpan 90
		End

		Event
			At Root
			Type StartPositOnly
			Part :Dirt_Stream_Out.part
		End

		Event
			At Root
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part :Dust_Lingering.part
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Local
			EName DirtAfter
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part :RocksScatter.part
			Part :DirtExplosion.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 45

		Event
			At Chest
			Type Start
			EName PHYSICS-ExplosiveForce-
			BhvrOverride
				physForceType Out
				physForceRadius 15
				physForcePower 600
				physForcePowerJitter 75
			End
			LifeSpan 1
		End

	End

End
