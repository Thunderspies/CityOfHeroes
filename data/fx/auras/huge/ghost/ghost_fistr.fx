#############################################################

## 			GHOSTS AURA

#############################################################

FxInfo
Flags DontSuppress

#########################################################
# 		SWIRLING GHOSTS

Condition
	On	Cycle
	Time	80

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

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Ghost_Smoke4.part

	End

	Event
		Type	Local
		At	HandR
		Part	:Ghost_MSmoke4.part

	End
End

#############################################################


End