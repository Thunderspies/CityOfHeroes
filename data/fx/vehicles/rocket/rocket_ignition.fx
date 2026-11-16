#########################################################
##	Warburg Rocket - Launch
##

FxInfo

Condition

	On Cycle 
	Time 1

	Event
		EName 	Cloud
		Type	Start
		Bhvr	:IgnitionRotation.bhvr
		part	:Rocket_ignition_Fire.part
		part	:Rocket_ignition_Smoke.part
		Lifespan	100
	End

End

End