#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo
Lifespan 50

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound Plasma_Welding_Ray_Hit_Loop 100 100 .25
	End

	Event
		Type	StartPositOnly
		At	Chest
		Part	:Hit_Plasma_Burst.part
		Part	:Hit_Smoke_Quick.part
	#	Part	:Hit_Glow_Drone.part
		Part	:Hit_Shockwave_Drone.part
		Part	:Hit_Smoke_Lingering.part
	End

	Event
		Type	Local
		At	SpadL
		Part	:Continuing_Plasma_Drone.part
		Lifespan 	60
		LifespanJitter	30
	End

	Event
		Type	Local
		At	SpadR
		Part	:Continuing_Plasma_Drone.part
		Lifespan 	60
		LifespanJitter	30
	End

	Event
		Type	Local
		At	Chest
		Part	:Continuing_Plasma_Rings_Drone.part
		Part	:Continuing_Plasma_Drone.part
		Lifespan 	60
		LifespanJitter	30
	End

	Event
		Type	Local
		At	Hips
		Part	:Continuing_Plasma_Drone.part
		Lifespan 	60
		LifespanJitter	30
	End
End

#############################################################

End