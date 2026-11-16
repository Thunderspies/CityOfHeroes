#########################################################
##	Mirror teleport
########################################################

FxInfo

Lifespan 60

########  offset for mirror spawn  #################

Condition
	On	Time
	Time	0

	Event
		EName	Offset
		Type	Local
		At	Root
		#Geom	Testing_Target
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			Behavior
			PositionOffset		0 .7 0
		End
	End
End

####  blobblt thing   ########################################

Condition
	On 	time
	Time 	0

	Event
		Ename	test
		Type	Local
		At	Offset  ##Root
		Anim	FX_mirror_teleport
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			FadeOutLength		40
		End
	End
End

#############################################################

End