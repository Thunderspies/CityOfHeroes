#############################################################
## GCDistortionContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	Chest
		part	:GCLiftingElement1out.part
		Pmagnet Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	End

End



Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	HandL
		part	:GCLiftingElement1outSmall.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	HandR
		part	:GCLiftingElement1outSmall.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	FootL
		part	:GCLiftingElement1outSmall.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	FootR
		part	:GCLiftingElement1outSmall.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

#############################################################

End

