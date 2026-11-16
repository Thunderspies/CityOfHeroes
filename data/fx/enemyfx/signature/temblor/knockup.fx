#############################################################
## TEMBLOR KNOCK UP FX
#############################################################

FxInfo
Lifespan	180
#############################################################


Condition
	On 	Time
	Time 	60

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\CameraShake_Subtle90.bhvr
		BhvrOverride
			PositionOffset		0 0.4 0
		End
		Part	:Dust_Ground.part
		Part	:Dust_Ground_Dark.part
		Lifespan	30
	End

	Event
		Type	Local
		At	Root
		Sound 	Earthquake_Knockup_01 200 200 .8
	End

End

##

Condition
	On 	Time
	Time 	65

	Event
		Ename	ChunkUp01
		Type	Local
		At	Root
		Bhvr	behaviors\GenericEarthFade.bhvr
		BhvrOverride
			Scale			0.5 0.3 0.5
			PositionOffset 		10 -10 15
			PyrRotateJitter		0 180 0
			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0 0.1 0
			Gravity			-0.6

			PhysForceType		Up
			PhysForcePower		1000
		End
		Geom	Ground_Chunk_01
		Lifespan 40
	End

	Event
		Type	Local
		At	Root
		Part	:RockBits_03.part
		Bhvr	behaviors\CameraShake_Strong30.bhvr
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	65

	Event
		Type	Local
		At	Root
		Part	:RockBits_04.part
		Part	:RockBits_05.part
		Part	:AirStreaks.part

		Lifespan	25
	End
End


Condition
	On 	Time
	Time 	65

	Event
		Type	Local
		At	Root
		Part	:Dust_Up.part
		Part	:Dust_Up_Dark.part
		Lifespan	30
	End
End

Condition
	On 	Time
	Time 	65

	Event
		Type	Local
		At	Root
		Part	:Dust_01.part
		Part	:Dust_Dark01.part
		Lifespan	40
	End
End

Condition
	On	Time
	Time	65

	Event
		Type	StartPositOnly
		At	ChunkUp01
		BhvrOverride
			PositionOffset		0 10.3 0
		End
		Part	:Cracks.part
		Lifespan	180
	End

End

Condition
	On	Time
	Time	70
	Event
		Ename	ChunkUp02
		Type	Local
		At	Root

		BhvrOverride
			Scale			0.3 0.3 0.3
			PositionOffset 		-15 -10 -10
			PyrRotateJitter		0 180 0
			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0 0.1 0
			Gravity			-0.6
			FadeOutLength 		30
			PhysForceType		Up
			PhysForcePower		1000
		End
		Geom	Ground_Chunk_02
		Lifespan 40
	End

	Event
		Type	StartPositOnly
		At	ChunkUp02
		BhvrOverride
			PositionOffset		0 10.3 0
		End
		Part	:Cracks.part
		Lifespan	180
	End
End

Condition
	On	Time
	Time	75
	Event
		Ename	ChunkUp03
		Type	Local
		At	Root
		Bhvr	behaviors\GenericEarthFade.bhvr
		BhvrOverride
			Scale			0.3 0.4 0.3
			PositionOffset 		-5 -10 8
			PyrRotateJitter		0 180 0
			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0 0.1 0
			Gravity			-0.6

			PhysForceType		Up
			PhysForcePower		1000
		End
		Geom	Ground_Chunk_01
		Lifespan 40
	End

	Event
		Type	StartPositOnly
		At	ChunkUp03
		BhvrOverride
			PositionOffset		0 10.3 0
		End
		Part	:Cracks.part
		Lifespan	180
	End
End

Condition
	On	Time
	Time	80
	Event
		Ename	ChunkUp04
		Type	Local
		At	Root
		Bhvr	behaviors\GenericEarthFade.bhvr
		BhvrOverride
			Scale			0.15 0.2 0.15
			PositionOffset 		0 -10 0
			StartJitter 		15 0 15
			PyrRotateJitter		0 180 0
			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0 0.1 0
			Gravity			-0.5

			PhysForceType		Up
			PhysForcePower		1000
		End
		Geom	Ground_Chunk_01
		Lifespan 40
	End

	Event
		Type	StartPositOnly
		At	ChunkUp04
		BhvrOverride
			PositionOffset		0 10.3 0
		End
		Part	:Cracks.part
		Lifespan	180
	End
End

Condition
	On	Time
	Time	85
	Event
		Ename	ChunkUp05
		Type	Local
		At	Root
		Bhvr	behaviors\GenericEarthFade.bhvr
		BhvrOverride
			Scale			0.3 0.2 0.3
			PositionOffset 		0 -10 0
			StartJitter 		20 0 20
			PyrRotateJitter		0 180 0
			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0 0.1 0
			Gravity			-0.5

			PhysForceType		Up
			PhysForcePower		1000
		End
		Geom	Ground_Chunk_02
		Lifespan 40
	End

	Event
		Type	StartPositOnly
		At	ChunkUp05
		BhvrOverride
			PositionOffset		0 10.3 0
		End
		Part	:Cracks.part
		Lifespan	180
	End
End

Condition
	On	Time
	Time	90
	Event
		Ename	ChunkUp06
		Type	Local
		At	Root
		Bhvr	behaviors\GenericEarthFade.bhvr
		BhvrOverride
			Scale			0.2 0.3 0.2
			PositionOffset 		8 -10 -12
			PyrRotateJitter		0 180 0
			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0 0.1 0
			Gravity			-0.6

			PhysForceType		Up
			PhysForcePower		1000
		End
		Geom	Ground_Chunk_02
		Lifespan 40
	End

	Event
		Type	StartPositOnly
		At	ChunkUp06
		BhvrOverride
			PositionOffset		0 10.3 0
		End
		Part	:Cracks.part
		Lifespan	180
	End
End



#############################################################-

End