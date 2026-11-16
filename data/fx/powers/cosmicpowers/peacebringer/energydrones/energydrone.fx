#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	coreD
		Type	Local
		At	mystic
		Bhvr	:Lightform.bhvr
		Sound glow9b_loop 70 70 1.0
	End

	Event
		EName	core
		Type	Local
		At	coreD
		Bhvr	:LightformA.bhvr
		Part	:LightTendrils.part
		Part	:Sparks.part
		Sound peacematter 70 70 .7
	End

	Event
		Type	Local
		At	root
		Bhvr	:Lightform2.bhvr
		Sound eleczaps_loop 70 70 .1
	End
End

#############################################################

End