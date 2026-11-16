#########################################################
##	template

FxInfo


Flags    InheritAlpha

LifeSpan 	81
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
##		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	FX_clawspeed
	End
End

#################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		Geom	V_QuillLeft
		LifeSpan 	36
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		Geom	V_QuillRight
		LifeSpan 	36
			
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