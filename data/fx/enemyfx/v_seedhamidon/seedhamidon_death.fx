#############################################################
## SeedHamidon_Aura.fx
#############################################################

FxInfo
#Lifespan 200

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SeedOfHamidon_DeathVOX_01 3000 500 1
	End
End

Condition
	On 	Time
	Time	85
	Event
		Type	Local
		At	Root
		Sound 	SeedOfHamidon_Death_01 2000 1000 .8
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		part	:HamiDeath_Flash_Initial.part
		Lifespan 90
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Ename	TailBurst6
		Type	Local
		At	Rwing4
		BhvrOverride
			PositionOffset	0 0 -1.5
			Scale	0.0005 0.0005 0.0005
		End
		ChildFX	:SeedHamidon_TailDeath_Child.fx
		Lifespan 55
	End
End

Condition
	On 	Time
	Time 	50

	Event
		Ename	TailBurst5
		Type	Local
		At	Rwing4
		BhvrOverride
			Scale	0.0005 0.0005 0.0005
		End
		ChildFX	:SeedHamidon_TailDeath_Child.fx
		Lifespan 50
	End

	Event
		Type	Local
		At	Hips
		Bhvr	:CameraShake_WideRange_Subtle.bhvr
		Lifespan 90
	End
End

Condition
	On 	Time
	Time 	55

	Event
		Ename	TailBurst4
		Type	Local
		At	Rwing3
		BhvrOverride
			Scale	0.001 0.001 0.001
		End
		ChildFX	:SeedHamidon_TailDeath_Child.fx
		Lifespan 45
	End
End

Condition
	On 	Time
	Time 	60

	Event
		Ename	TailBurst3
		Type	Local
		At	Rwing2
		BhvrOverride
			Scale	0.01 0.01 0.01
		End
		ChildFX	:SeedHamidon_Death_Child.fx
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	65
#
	Event
		Ename	TailBurst2
		Type	Start
		At	Rwing1
		BhvrOverride
			PositionOffset	0 50 0
			Scale	0.003 0.003 0.003
		End
		ChildFX	:SeedHamidon_Death_Child.fx
		Lifespan 35
	End

	Event
		Ename	TailBurst1
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	-1.8 -0.2 -0.6
			Scale		0.05 0.05 0.05
		End
		ChildFX	:SeedHamidon_HeadDeath_Child.fx
		Lifespan 35
	End

	Event
		Ename	TailBurst1b
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	1.8 -0.2 -0.6
			Scale		0.05 0.05 0.05
		End
		ChildFX	:SeedHamidon_HeadDeath_Child.fx
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	70

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0 -0.3 1
			Scale		0.1 0.1 0.1
		End
		ChildFX	:SeedHamidon_HeadDeath_Child.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	85

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0 -0.3 1
			Scale		0.1 0.1 0.1
		End
		Part	:Splash_InnerSplatter.part
		Lifespan 15
	End
End

##########################

Condition
	On 	Time
	Time 	95

	Event
		Ename	ChunkCenter
		Type	Local
		At	Hips
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			Scale		1 1 1
			PositionOffset	0.0 -0.05 -0.8
		End
		part	:HamiDeath_Streaks_Initial.part
	End
End

Condition
	On 	Time
	Time 	100

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	:CameraShake_WideRange_Strong.bhvr
	#	Flags	AdoptParentEntity
		part	:HamiDeath_Flash_Big.part
		part	:HamiDeath_Streaks_Big.part
		part	:HamiDeath_Streaks_Big_Alpha.part
		Part	:HamiDeath_Shockwave_Big.part
		Part	:HamiDeath_Sparks_Big.part
		Part	:HamiDeath_Splashes.part
		Part	:HamiDeath_Splatter.part
		Part	:HamiDeath_Chunks.part
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 1 3
			InitialVelocity		0.0 0.2 0.2
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			-0.05 -0.01 0
			SpinJitter		0.01 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk01
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 1.9 2
			InitialVelocity		0.0 0.4 0.15
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			-0.05 -0.01 0
			SpinJitter		0.01 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk02
		Lifespan 15
	End
	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.75 -1.5 3.65
			InitialVelocity		-0.2 0.15 0.1
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			-0.05 -0.01 0
			SpinJitter		0.01 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk03
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.65 -0.5 3.5
			InitialVelocity		-0.1 0.15 0.2
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			-0.05 -0.01 0
			SpinJitter		0.01 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk04
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-0.45 -0.5 2.3
			InitialVelocity		-0.2 -0.1 0.1
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			-0.05 -0.01 0
			SpinJitter		0.01 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk05
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1.18 -0.56 2.4
			InitialVelocity		-0.1 -0.0 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.06
			Drag			0.02
			Spin			-0.1 -0.1 0
			SpinJitter		0.05 0.08 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk06
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1.75 -0.67 2.65
			InitialVelocity		-0.2 0.3 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.1 0
			SpinJitter		0.3 0.08 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk07
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1.23 0.05 1.95
			InitialVelocity		-0.2 0.3 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.1 0
			SpinJitter		0.05 0.08 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk08
		Lifespan 15
	End



	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.0 0.7 1.3
			InitialVelocity		-0.3 0.55 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk14
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.15 0.45 1.45
			InitialVelocity		-0.35 0.6 0.0
			InitialVelocityJitter	0.1 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk15
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1.5 1.7 -0.4
			InitialVelocity		-0.15 0.5 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk16
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.2 1.5 0
			InitialVelocity		-0.25 0.5 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.05
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk17
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-3.1 1.05 -0.6
			InitialVelocity		-0.2 0.4 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.05
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk18
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.2 0 -0.9
			InitialVelocity		-0.05 -0.2 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk19
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-3.74 0.61 -0.2
			InitialVelocity		-0.4 0.2 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk20
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1.4 2.22 -2.25
			InitialVelocity		-0.0 0.4 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk21
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.58 0.75 -1.48
			InitialVelocity		-0.1 0.05 -0.1
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk22
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1.54 1.15 -2.53
			InitialVelocity		-0.1 0.05 -0.15
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk23
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.68 2.25 -2.72
			InitialVelocity		-0.1 0.2 -0.25
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk24
		Lifespan 15
	End

#################


	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.75 -1.5 3.65
			InitialVelocity		0.2 0.15 0.1
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.05
			Drag			0.02
			Spin			-0.05 -0.01 0
			SpinJitter		0.01 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk003
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.65 -0.5 3.5
			InitialVelocity		0.1 0.15 0.2
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			-0.05 -0.01 0
			SpinJitter		0.01 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk004
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0.45 -0.5 2.3
			InitialVelocity		0.2 -0.1 0.1
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			-0.05 -0.01 0
			SpinJitter		0.01 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk005
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.18 -0.56 2.4
			InitialVelocity		0.1 -0.0 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.06
			Drag			0.02
			Spin			-0.1 -0.1 0
			SpinJitter		0.05 0.08 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk06
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.75 -0.67 2.65
			InitialVelocity		0.2 0.3 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.1 0
			SpinJitter		0.3 0.08 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk007
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.23 0.05 1.95
			InitialVelocity		0.2 0.3 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.1 0
			SpinJitter		0.05 0.08 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk008
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.6 1.15 1.3
			InitialVelocity		0.15 0.35 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.1 0
			SpinJitter		0.05 0.08 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk008b
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.0 -0.15 1
			InitialVelocity		0.15 0.35 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk009
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		3.2 -1 0.9
			InitialVelocity		0.25 0.35 -0.05
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk010
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		3.5 -1.2 1.5
			InitialVelocity		0.3 0.45 0.0
			InitialVelocityJitter	0.07 0.1 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk011
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1 2.0 1.0
			InitialVelocity		0.05 0.65 0.0
			InitialVelocityJitter	0.1 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk012
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.0 1.1 1.1
			InitialVelocity		0.4 0.7 0.0
			InitialVelocityJitter	0.1 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk013
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.0 0.7 1.3
			InitialVelocity		0.3 0.55 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk014
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.15 0.45 1.45
			InitialVelocity		0.35 0.6 0.0
			InitialVelocityJitter	0.1 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk015
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.5 1.7 -0.4
			InitialVelocity		0.15 0.5 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk016
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.2 1.5 0
			InitialVelocity		0.25 0.5 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.05
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk017
		Lifespan 20
	End

###############################

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1.6 1.15 1.3
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk08b
		Lifespan 5
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.0 -0.15 1
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk09
		Lifespan 5
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-3.2 -1 0.9
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk10
		Lifespan 5
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-3.5 -1.2 1.5
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk11
		Lifespan 5
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1 2.0 1.0
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk12
		Lifespan 5
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.0 1.1 1.1
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk13
		Lifespan 5
	End

End

Condition
	On 	Time
	Time 	98

Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1.6 1.15 1.3
			InitialVelocity		-0.15 0.35 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.1 0
			SpinJitter		0.05 0.08 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk08b
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.0 -0.15 1
			InitialVelocity		-0.15 0.35 0.1
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk09
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-3.2 -1 0.9
			InitialVelocity		-0.25 0.35 -0.05
			InitialVelocityJitter	0.07 0.05 0.08
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk10
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-3.5 -1.2 1.5
			InitialVelocity		-0.3 0.45 0.0
			InitialVelocityJitter	0.07 0.1 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk11
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-1 2.0 1.0
			InitialVelocity		-0.05 0.65 0.0
			InitialVelocityJitter	0.1 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk12
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		-2.0 1.1 1.1
			InitialVelocity		-0.4 0.7 0.0
			InitialVelocityJitter	0.1 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk13
		Lifespan 20
	End

End

################################

Condition
	On 	Time
	Time 	100

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		3.1 1.05 -0.6
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk018
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.2 0 -0.9
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk019
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		3.74 0.61 -0.2
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk020
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.4 2.22 -2.25
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk021
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.58 0.75 -1.48
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk022
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.54 1.15 -2.53
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk023
		Lifespan 10
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.68 2.25 -2.72
			FadeOutLength	1
		End
		Geom	FX_SeedHamidon_Chunk024
		Lifespan 10
	End

End

Condition
	On 	Time
	Time 	120

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		3.1 1.05 -0.6
			InitialVelocity		0.2 0.4 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.05
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk018
		Lifespan 20
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.2 0 -0.9
			InitialVelocity		0.05 -0.2 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.05 0.008 0.05
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk019
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		3.74 0.61 -0.2
			InitialVelocity		0.4 0.2 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk020
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.4 2.22 -2.25
			InitialVelocity		0.0 0.4 0.0
			InitialVelocityJitter	0.05 0.05 0.0
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk021
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.58 0.75 -1.48
			InitialVelocity		0.1 0.05 -0.1
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk022
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		1.54 1.15 -2.53
			InitialVelocity		0.1 0.05 -0.15
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk023
		Lifespan 15
	End

	Event
		Type	Local
		At	ChunkCenter
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		2.68 2.25 -2.72
			InitialVelocity		0.1 0.2 -0.25
			InitialVelocityJitter	0.05 0.05 0.05
			Gravity			0.03
			Drag			0.02
			Spin			0.0 -0.01 0
			SpinJitter		0.1 0.008 0.1
			FadeOutLength	15
		End
		Geom	FX_SeedHamidon_Chunk024
		Lifespan 15
	End

End

#############################################################
End