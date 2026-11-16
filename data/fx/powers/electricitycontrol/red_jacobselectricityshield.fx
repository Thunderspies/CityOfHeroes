#############################################################
## Red_JacobsElectricityShield.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound elecbuildup3 70 70 .7
	End

	Event
		Type	Local
		At	FootL
		ChildFx	POWERS\ElectricityControl\Red_JacobsElectricityShieldChild.fx
		LifeSpan 10
	End
End

#############################################################

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		Sound elecarmor4_loop 70 70 .3
		bhvr	behaviors\soundFade6.bhvr
		Flags	PowerLoopingSound
		Lifespan 350
	End
End

#############################################################

Condition
	On	Cycle
	Time	30

	Event
		Type	Local
		At	FootL
		ChildFx	POWERS\ElectricityControl\Red_JacobsElectricityShieldChild.fx
		LifeSpan 10
	End
End

#############################################################

End