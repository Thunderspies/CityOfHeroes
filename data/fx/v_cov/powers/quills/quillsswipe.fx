#########################################################
##	template

FxInfo


LifeSpan 	62

Flags    InheritAlpha

#########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	Root
		Sound thornywhoosh 70 70 .4
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
##		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	FX_clawspeed
		
	End
	
	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		Geom	V_QuillLeft
		LifeSpan 	42
			
	End
	
	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		Geom	V_QuillRight
		LifeSpan 	42	
	End

End

###############################################################################
##SpikeShrink
###################################

Condition
	On 	Time
	Time 	47

	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/QuillSpikeScaleDown.bhvr
		Geom	V_QuillLeft
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScaleDown.bhvr
		Geom	V_QuillRight
			
	End
End

##############################################################################

End		