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
		sound PPD_MonitorSphereHitReact_01 100 100 .8
		sound PPD_MonitorSphereHitReact_02 100 100 .8
		sound PPD_MonitorSphereHitReact_03 100 100 .8
		sound PPD_MonitorSphereHitReact_04 100 100 .8
		sound PPD_MonitorSphereHitReact_05 100 100 .8

	End

End

#############################################################
