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
		sound PPD_MonitorSphere_Explode_01 200 200 .8
	End

End

#############################################################
