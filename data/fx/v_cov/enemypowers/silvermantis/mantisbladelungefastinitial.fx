#########################################################
##	template

FxInfo


Flags    InheritAlpha

LifeSpan 	81
#########################################################

Condition
	On 	Time
	Time 	10

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
	Time 	10

	Event
		EName	Spike
		Type	Local
		At	HandL
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		BhvrOverride
			FadeInLength		20
			FadeOutLength		5

			Scale			.1 .1 .1
			ScaleRate		1.05 1.05 1.05
			 EndScale		.8 1 .8
		End
		Geom	Mantis_Silver_Blade_Left
		LifeSpan 	36
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScaleQuick.bhvr
		BhvrOverride
			FadeInLength		20
			FadeOutLength		5

			Scale			.1 .1 .1
			ScaleRate		1.05 1.05 1.05
			 EndScale		.8 1 .8
		End
		Geom	Mantis_Silver_Blade_Right
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
		BhvrOverride
			FadeInLength		5
			FadeOutLength		40

			Scale			.8 1 .8
			ScaleRate		1.05 1.05 1.05
			 EndScale		.1 .1 .1
		End
		Geom	Mantis_Silver_Blade_Left
			
	End

	Event
		EName	Spike
		Type	Local
		At	HandR
		
		Bhvr	Behaviors/QuillSpikeScaleDown.bhvr
		BhvrOverride
			FadeInLength		5
			FadeOutLength		40

			Scale			.8 1 .8
			ScaleRate		1.05 1.05 1.05
			 EndScale		.1 .1 .1
		End
		Geom	Mantis_Silver_Blade_Right
			
	End
End

##############################################################################

End		