#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	WepR

		Sound rockpickup7 100.0 100.0 .85
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	chunk
		Type	local
		At	WepR
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:projectileLargeSlagChunk.bhvr

	End

	Event
		EName	junk1
		Type	local
		At	chunk
		geom	GEO_Slag_Chunk_Small

	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	cracks
		Type	start
		At	WepR
		Part	:SlagDustRocks2.part
		Part2	:SlagDust1.part
	End


	Event
		EName	debris
		Type	StartPositOnly
		At	WepR
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
		ChildFX	:SlagSmallChunkDebrisTrail.fx
		Sound Rockthrow 100 100 .9


	End


	Event

		EName 	grime
		Type	Local
		At	Prime

		geom	GEO_Slag_Chunk_Small


	End


	Event
		EName	dust
		Type	local
		At	Prime
		part	:SlagDustTrail.part
		part	:SlagDustTrail2.part
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

