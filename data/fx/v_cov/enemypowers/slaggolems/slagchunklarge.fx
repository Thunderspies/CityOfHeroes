#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	Mystic

		Sound rockpickup7 100.0 100.0 .85
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	chunk
		Type	local
		At	Mystic
		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_03
		geom	GEO_Slag_Golem_Junk_04
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_11
		geom	GEO_Slag_Golem_Junk_45
		bhvr	:projectileLargeSlagChunk.bhvr

	End

	Event
		EName	junk1
		Type	local
		At	chunk
		geom	GEO_Slag_Golem_Junk_12

	End

	Event
		EName	junk1
		Type	local
		At	chunk
		geom	GEO_Slag_Chunk_Large

	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	cracks
		Type	start
		At	Mystic
		bhvr	behaviors/CameraShakeBoulderHurl.bhvr
		Part	:SlagDustRocks2.part
		Part2	:SlagDust1.part
		part3	:SlagDust2.part
	End
End

Condition
	On 	Time
	Time 	25
	Repeat	2

	Event
		EName	debris
		Type	StartPositOnly
		At	Mystic
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisTrails.bhvr
		Lifespan	150
		LifespanJitter	30

	End

End

Condition
	On 	Time
	Time 	93

	Event
		EName	chunk
		Type	Destroy

	End

End

Condition
	On 	Time
	Time 	92

	Event

		EName 	Prime
		Type	Start
		At	chunk

		geom	Parent
		Bhvr	:projectileLargeSlagChunk2.bhvr
		Magnet	Target
		LookAt	Target
		ChildFX	:SlagLargeChunkDebrisTrail.fx
		Sound Rockthrow 100 100 .9


	End


	Event

		EName 	grime
		Type	Local
		At	Prime

		geom	GEO_Slag_Chunk_Large


	End

	Event
		EName	junk1
		Type	local
		At	Prime
		geom	GEO_Slag_Golem_Junk_12

	End


	Event
		EName	dust
		Type	local
		At	Prime
		part	:SlagDustTrail.part
		part	:SlagDustTrail2.part
		part	:SlagDustRocks.part
		part	:SlagDustRocks.part

	End


End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy


	End


End

Condition
	On 	Time
	Time 	200

	Event
		EName	All
		Type	Destroy

	End


End

End
##################################

