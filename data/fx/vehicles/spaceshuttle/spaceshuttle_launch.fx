#########################################################
##	Warburg Rocket - Launch
##

FxInfo

Condition

	Event
		EName 	Cloud
		Type	Start
		Bhvr	:IgnitionRotation.bhvr
		part	:Rocket_booster_fire.part
	End

	Event
		EName 	Cloud
		Type	Start
		Bhvr	:IgnitionRotation.bhvr
		part	:Rocket_ignition_Sparks.part
		Lifespan	35
	End

End

Condition

	On Cycle 
	Time 1

	Event
		EName 	Cloud
		Type	local
		Bhvr	:IgnitionRotation.bhvr
		part	:Rocket_ignition_FireLicks.part
		
		Lifespan	100
	End

End

Condition

	On Cycle 
	Time 1

	Event
		EName 	Cloud
		Type	local
#		Bhvr	:IgnitionRotation.bhvr

		Part	:Rocket_ignition_CoreFire.part
		part	:Rocket_ignition_Fire.part

		Lifespan	30
	End

End

##############################################################
##Smoke
#############################################

Condition

	Event
		EName 	Cloud
		Type	local
		
		Childfx	:SpaceShuttle_Smoke.fx
		LifeSpan	130
	End

End

End