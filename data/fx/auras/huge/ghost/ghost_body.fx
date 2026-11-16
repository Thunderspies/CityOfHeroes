#############################################################

## 			GHOSTS AURA

#############################################################

FxInfo
Flags DontSuppress

#########################################################
# 		SWIRLING GHOSTS

Condition
	On	Cycle
	Time	20
	Random	1

	Event
		Type	Local
		At	Origin
		ChildFX	:Ghost_Child_01.fx
		Lifespan 85
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:Ghost_Child_02.fx
		Lifespan 85
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:Ghost_Child_03.fx
		Lifespan 85
	End

	Event
		Type	Local
		At	Origin
		ChildFX	:Ghost_Child_04.fx
		Lifespan 85
	End
End

#############################################################
#		EERIE SMOKE (IDLE)

Condition
	On	Time
	Time	0
	Random	0

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke.part

	End

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke.part

	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke2.part

	End

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke2.part

	End

		Event
		Type	Local
		At	FootL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke3.part

	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke3.part

	End
End

###########################################
#	 	EERIE SMOKE (ANIM)

Condition
	On	Time
	Time	0
	Random	0

	Event
		Type	Local
		At	Chest
		Part	:Ghost_MSmoke.part

	End

	Event
		Type	Local
		At	Hips
		Part	:Ghost_MSmoke.part

	End


	Event
		Type	Local
		At	LLegL
		Part	:Ghost_MSmoke2.part

	End

	Event
		Type	Local
		At	LLegR
		Part	:Ghost_MSmoke2.part

	End

End

#############################################################


End