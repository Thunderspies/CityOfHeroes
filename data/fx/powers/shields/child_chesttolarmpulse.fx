#############################################################
## OneWithTheShield_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	HeartOffset
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 0.25 0
		End
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	RingL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall2.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	5

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
End

Condition
	On	Time
	Time	10

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
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	HeartOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Pulse_GlowSmall1.part
		Part	:Shields_Pulse_Ripple1.part
		Part	:Shields_Pulse_Ripple2.part
		pOther	UArmL
		Lifespan 10
	End
End

#Condition
#	On	Time
#	Time	20
#
#	Event
#		Type	Local
#		At	HeartOffset
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		Part	:Shields_Pulse_GlowSmall1.part
#		Part	:Shields_Pulse_Ripple1.part
#		Part	:Shields_Pulse_Ripple2.part
#		pOther	Waist
#		Lifespan 10
#	End
#End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	HeartOffset
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Shields_Activation_CoreGlow1.part
		Part	:Shields_Activation_CoreStar1.part
		Lifespan 10
	End
End

#############################################################

End