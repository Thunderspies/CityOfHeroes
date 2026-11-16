#
FxInfo

	LifeSpan 240


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ToV_EarthSibyl_Earthquake_01 300 100 .8
	End
End


	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Bhvr behaviors\CameraShake_Subtle90.bhvr
			BhvrOverride
				PositionOffset 0 0.4 0
			End
			Part :Dust_Ground.part
			Part :Dust_Ground_Dark.part
			#Part :InitRockScatter02.part
			#Part :Dirt_InitLingering.part
			LifeSpan 30
		End
	End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			Part :InitRockScatter.part
			#Part :Dirt_InitLingering.part
			Part :Dirt_InitStream_Down.part
			Part :Dirt_InitStream_Down.part
			LifeSpan 30
		End
	End

	Condition
		On Time
		Time 20

		Event
			At Root
			Type Local
			Part :InitRockScatter.part
			#Part :Dirt_InitLingering.part
			Part :Dirt_InitStream_Down.part
			LifeSpan 30
		End
	End

	Condition
		On Time
		Time 30

		Event
			At Root
			Type Local
			Part :InitRockScatter02.part
			#Part :Dirt_InitLingering.part
			Part :Dirt_InitStream_Down.part
			LifeSpan 30
		End
	End

	Condition
		On Time
		Time 40

		Event
			At Root#
			Type Local
			Part :InitRockScatter.part
			#Part :Dirt_InitLingering.part
			Part :Dirt_InitStream_Down.part
			LifeSpan 30
		End
	End

	Condition
		On Time
		Time 55

		Event
			At Root
			Type Local
			EName ChunkUp01
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.5 0.3 0.5
				PositionOffset 10 -10 15
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			Bhvr behaviors\CameraShake_Strong30.bhvr
			Part :RockBits_03.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :RockBits_04.part
			Part :RockBits_05.part
			Part :AirStreaks.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			Part :Dust_Up.part
			Part :Dust_Up_Dark.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :Dust_01.part
			LifeSpan 40
		End

		Event
			At ChunkUp01
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
		#	Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 60

		Event
			At Root
			Type Local
			EName ChunkUp02
			BhvrOverride
				Scale 0.3 0.3 0.3
				PositionOffset -10 -10 -10
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				FadeOutLength 30
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp02
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 65

		Event
			At Root
			Type Local
			EName ChunkUp03
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.3 0.4 0.3
				PositionOffset -5 -10 8
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At ChunkUp03
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 70

		Event
			At Root
			Type Local
			EName ChunkUp04
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.15 0.2 0.15
				PositionOffset 5 -10 0
				StartJitter 5 0 5
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.5
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At ChunkUp04
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 75

		Event
			At Root
			Type Local
			EName ChunkUp05
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.3 0.2 0.3
				PositionOffset 0 -10 5
				StartJitter 5 0 5
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.5
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp05
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 80

		Event
			At Root
			Type Local
			EName ChunkUp06
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.2 0.3 0.2
				PositionOffset 8 -10 -12
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp06
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 85

		Event
			At Root
			Type Local
			EName ChunkUp01
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.5 0.3 0.5
				PositionOffset 10 -10 15
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			Bhvr behaviors\CameraShake_Strong30.bhvr
			Part :RockBits_03.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :RockBits_04.part
			Part :RockBits_05.part
			Part :AirStreaks.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			Part :Dust_Up.part
			Part :Dust_Up_Dark.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :Dust_01.part
			LifeSpan 40
		End

		Event
			At ChunkUp01
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
		#	Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 90

		Event
			At Root
			Type Local
			EName ChunkUp02
			BhvrOverride
				Scale 0.3 0.3 0.3
				PositionOffset -10 -10 -10
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				FadeOutLength 30
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp02
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 95

		Event
			At Root
			Type Local
			EName ChunkUp03
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.3 0.4 0.3
				PositionOffset -5 -10 8
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At ChunkUp03
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 100

		Event
			At Root
			Type Local
			EName ChunkUp04
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.15 0.2 0.15
				PositionOffset 5 -10 0
				StartJitter 5 0 5
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.5
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At ChunkUp04
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 105

		Event
			At Root
			Type Local
			EName ChunkUp05
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.3 0.2 0.3
				PositionOffset 0 -10 5
				StartJitter 5 0 5
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.5
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp05
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 110

		Event
			At Root
			Type Local
			EName ChunkUp06
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.2 0.3 0.2
				PositionOffset 8 -10 -12
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp06
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 115

		Event
			At Root
			Type Local
			EName ChunkUp01
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.5 0.3 0.5
				PositionOffset 10 -10 15
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			Bhvr behaviors\CameraShake_Strong30.bhvr
			Part :RockBits_03.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :RockBits_04.part
			Part :RockBits_05.part
			Part :AirStreaks.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			Part :Dust_Up.part
			Part :Dust_Up_Dark.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :Dust_01.part
			LifeSpan 40
		End

		Event
			At ChunkUp01
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
		#	Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 120

		Event
			At Root
			Type Local
			EName ChunkUp02
			BhvrOverride
				Scale 0.3 0.3 0.3
				PositionOffset -10 -10 -10
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				FadeOutLength 30
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp02
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 125

		Event
			At Root
			Type Local
			EName ChunkUp03
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.3 0.4 0.3
				PositionOffset -5 -10 8
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At ChunkUp03
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 130

		Event
			At Root
			Type Local
			EName ChunkUp04
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.15 0.2 0.15
				PositionOffset 5 -10 0
				StartJitter 5 0 5
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.5
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At ChunkUp04
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 135

		Event
			At Root
			Type Local
			EName ChunkUp05
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.3 0.2 0.3
				PositionOffset 0 -10 5
				StartJitter 5 0 5
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.5
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp05
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 140

		Event
			At Root
			Type Local
			EName ChunkUp06
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.2 0.3 0.2
				PositionOffset 8 -10 -12
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp06
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 145

		Event
			At Root
			Type Local
			EName ChunkUp01
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.5 0.3 0.5
				PositionOffset 10 -10 15
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			Bhvr behaviors\CameraShake_Strong30.bhvr
			Part :RockBits_03.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :RockBits_04.part
			Part :RockBits_05.part
			Part :AirStreaks.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			Part :Dust_Up.part
			Part :Dust_Up_Dark.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :Dust_01.part
			LifeSpan 40
		End

		Event
			At ChunkUp01
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
		#	Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 150

		Event
			At Root
			Type Local
			EName ChunkUp02
			BhvrOverride
				Scale 0.3 0.3 0.3
				PositionOffset -10 -10 -10
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				FadeOutLength 30
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp02
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 155

		Event
			At Root
			Type Local
			EName ChunkUp03
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.3 0.4 0.3
				PositionOffset -5 -10 8
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At ChunkUp03
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 160

		Event
			At Root
			Type Local
			EName ChunkUp04
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.15 0.2 0.15
				PositionOffset 5 -10 0
				StartJitter 5 0 5
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.5
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At ChunkUp04
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 165

		Event
			At Root
			Type Local
			EName ChunkUp05
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.3 0.2 0.3
				PositionOffset 0 -10 5
				StartJitter 5 0 5
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.5
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp05
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 170

		Event
			At Root
			Type Local
			EName ChunkUp06
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.2 0.3 0.2
				PositionOffset 8 -10 -12
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp06
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 175

		Event
			At Root
			Type Local
			EName ChunkUp01
			Bhvr behaviors\GenericEarthFade.bhvr
			BhvrOverride
				Scale 0.5 0.3 0.5
				PositionOffset 10 -10 15
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_01
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			Bhvr behaviors\CameraShake_Strong30.bhvr
			Part :RockBits_03.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :RockBits_04.part
			Part :RockBits_05.part
			Part :AirStreaks.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			Part :Dust_Up.part
			Part :Dust_Up_Dark.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Part :Dust_01.part
			LifeSpan 40
		End

		Event
			At ChunkUp01
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
		#	Part :Cracks.part
			LifeSpan 240
		End

	End

	Condition
		On Time
		Time 180

		Event
			At Root
			Type Local
			EName ChunkUp02
			BhvrOverride
				Scale 0.3 0.3 0.3
				PositionOffset -10 -10 -10
				PyrRotateJitter 0 180 0
				InitialVelocity 0 0.3 0
				InitialVelocityJitter 0 0.1 0
				Gravity -0.6
				FadeOutLength 30
				physForceType Up
				physForcePower 1000
			End
			Geom Ground_Chunk_02
			LifeSpan 40
		End

		Event
			At ChunkUp02
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10.3 0
			End
			Part :Cracks.part
			LifeSpan 240
		End

	End

End
