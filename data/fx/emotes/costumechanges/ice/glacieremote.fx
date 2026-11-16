#############################################################
## Cond Control - Chillblain
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	UArmLFX
		Type	StartPositOnly
		At	Head
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		Part	:snowfall.part
		POther	Neck
		Lifespan 30
	End

	Event
		EName	UArmLFX
		Type	StartPositOnly
		At	UArmL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	LarmL
		Lifespan 30
	End

	Event
		EName	UArmRFX
		Type	StartPositOnly
		At	UArmR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	LArmR
		Lifespan 30
	End

	Event
		EName	LArmLFX
		Type	StartPositOnly
		At	LArmL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	HandL
		Lifespan 30
	End

	Event
		EName	LArmRFX
		Type	StartPositOnly
		At	LArmR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	HandR
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Ename	BodyFrost
		Type	StartPositOnly
		At	UlegL
		BhvrOverride
			PositionOffset	0 -1 0
		End
		Part	:snowfall.part
		Lifespan 30
	End

	Event
		Ename	BodyFrost
		Type	StartPositOnly
		At	UlegR
		BhvrOverride
			PositionOffset	0 -1 0
		End
		Part	:snowfall.part
		Lifespan 30
	End

	Event
		EName	UArmLFX
		Type	StartPositOnly
		At	Chest
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		Part	:snowfall.part
		POther	Hips
		Lifespan 30
	End

	Event
		EName	ULegLFX
		Type	StartPositOnly
		At	ULegL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	LLegL
		Lifespan 30
	End

	Event
		EName	ULegRFX
		Type	StartPositOnly
		At	ULegR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	LLegR
		Lifespan 30
	End

	Event
		EName	FootLFX
		Type	StartPositOnly
		At	LLegL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	FootL
		Lifespan 30
	End

	Event
		EName	FootRFX
		Type	StartPositOnly
		At	LLegR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	FootR
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	0
	Event
		Ename	BodyFrost
		Type	StartPositOnly
		At	Root
		ChildFx	:BodyFrost.fx
	End
End

##########################

Condition
	On 	Time
	Time 	35

	Event
		EName 	Rockup
		Type	StartPositOnly
		At	Hips
		Bhvr	:Geo_Ice_FadeIn.bhvr
		BhvrOverride
			PositionOffset 2 0 0
		End
		Geom	Glacier_Chunk_01
		Geom	Glacier_Chunk_02
		Lifespan 40
	End

	Event
		EName 	Rockup
		Type	StartPositOnly
		At	Hips
		Bhvr	:Geo_Ice_FadeIn.bhvr
		BhvrOverride
			PositionOffset 2 0 2
		End
		Geom	Glacier_Chunk_01
		Geom	Glacier_Chunk_02
		Lifespan 40
	End

	Event
		EName 	Rockup
		Type	StartPositOnly
		At	Hips
		Bhvr	:Geo_Ice_FadeIn.bhvr
		BhvrOverride
			PositionOffset -2 0 0
		End
		Geom	Glacier_Chunk_01
		Geom	Glacier_Chunk_02
		Lifespan 40
	End

	Event
		EName 	Rockup
		Type	StartPositOnly
		At	Hips
		Bhvr	:Geo_Ice_FadeIn.bhvr
		BhvrOverride
			PositionOffset -2 0 -2
		End
		Geom	Glacier_Chunk_01
		Geom	Glacier_Chunk_02
		Lifespan 40
	End
End

########################

Condition
	On 	Time
	Time 	50
	Event
		Ename	BodyFrost
		Type	StartPositOnly
		At	Hips
		Part	:Glow.part
	End
End

Condition
	On	Time
	Time	72

	Event
		Ename Flash
		Type	Local
		At	Hips
		Part	:Blast.part
	End
End



#############################

Condition
	On	Time
	Time	75
	Repeat	14

	Event
		Ename	Kaboom
		Type	StartPositOnly
		At	Hips
		Bhvr	behaviors/FadeOut.bhvr

		BhvrOverride
			StartJitter		2 4 2
			Scale			0.1 0.15 0.1
			Spin 			0.2 0.1 0.1
			InitialVelocity 	0.0 1.2 0.0
			InitialVelocityJitter 	1.5 1 1.5

			FadeOutLength		45

			Drag	0.0
			Gravity	0.2

			physics 		1
			physGravity 		4
			physRestitution 	0.1
			physSFriction 	 	5
			physDFriction 	 	0.9
		End

		Geom	Glacier_Chunk_03
		Geom	Glacier_Chunk_04
		Geom	Glacier_Chunk_05
		Part	:IceBigScatter.part

		#CEvent	WORLD\tunnels\CHILD_RockBreak.fx
		Lifespan 90

	End
End

Condition
	On	Time
	Time	75
	Event
		Type	StartPositOnly
		At	Hips
		Part	:IceDust_Out.part
	End
End

Condition
	On 	Time
	Time 	72

	Event
		EName 	DirtAfter
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 0 5 0
		End
		part	:Ice_LingeringScatter.part
		part	:Ice_LingeringFrost.part
		Lifespan 30
	End
End


#############################################################

End