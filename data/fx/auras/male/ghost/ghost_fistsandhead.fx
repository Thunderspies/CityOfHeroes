#############################################################

## 			GHOSTS AURA

#############################################################

FxInfo
Flags DontSuppress

#########################################################
# 		SWIRLING GHOSTS
Condition
	On	Cycle
	Time	45
	Random	1

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Ghost_Fists.fx
		Lifespan 95
	End

	Event
		Type	Local
		At	Head
		ChildFX	:CHILD_Ghost_Fists02.fx
		Lifespan 95
	End
End

Condition
	On	Cycle
	Time	40
	Random	1

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Ghost_Fists.fx
		Lifespan 95
	End

	Event
		Type	Local
		At	HandL
		ChildFX	:CHILD_Ghost_Fists02.fx
		Lifespan 95
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Ghost_Fists.fx
		Lifespan 95
	End

	Event
		Type	Local
		At	HandR
		ChildFX	:CHILD_Ghost_Fists02.fx
		Lifespan 95
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
		At	HandL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke4.part

	End

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke4.part

	End

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke4.part

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
		At	HandL
		Part	:Ghost_MSmoke4.part

	End

	Event
		Type	Local
		At	HandR
		Part	:Ghost_MSmoke4.part

	End

	Event
		Type	Local
		At	Head
		Part	:Ghost_MSmoke4.part

	End

End

#############################################################


End