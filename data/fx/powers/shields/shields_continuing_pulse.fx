#########################################################
##  Willpower - Continuing Effect - Pulse (Child)
#########################################################

FxInfo

Lifespan 38

#########################################################

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	Waist
		Lifespan 10
	End

	Event
		Type	Local
		At	Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	UArmR
		Lifespan 10
	End

	Event
		Type	Local
		At	Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	UArmL
		Lifespan 10
	End
End

Condition
	On	Time
	Time	14

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	LArmR
		Lifespan 10
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	LArmL
		Lifespan 10
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowMedium1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	LLegR
		Lifespan 10
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowMedium1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	LLegL
		Lifespan 10
	End
End

Condition
	On	Time
	Time	21

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		Lifespan 10
		pOther	HandR
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		Lifespan 10
		pOther	HandL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowMedium1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	FootR
		Lifespan 10
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowMedium1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	FootL
		Lifespan 10
	End
End


Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	RingR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall2.part
		Lifespan 10
	End

	Event
		Type	Local
		At	RingL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall2.part
		Lifespan 10
	End

	Event
		Type	Local
		At	ToeR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall2.part
		Lifespan 10
	End

	Event
		Type	Local
		At	ToeL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall2.part
		Lifespan 10
	End
End

#########################################################

End