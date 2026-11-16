#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Geom	WaterBackPackSwitch
		Bhvr	CustomizeablePowers\Traps\FX_Detonator_small.bhvr
		Lifespan	60
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Start
		At	Root
		Sound M1 120 120 .46
		Lifespan 10
	End
End

#############################################################

End