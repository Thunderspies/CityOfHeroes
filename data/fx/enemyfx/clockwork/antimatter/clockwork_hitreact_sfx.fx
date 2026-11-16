#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
lifespan 1

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		sound Clockwork_HitReact_01 100 100 .8
		sound Clockwork_HitReact_02 100 100 .8
		sound Clockwork_HitReact_03 100 100 .8
		sound Clockwork_HitReact_04 100 100 .8
		sound Clockwork_HitReact_05 100 100 .8
	End

End

#############################################################
