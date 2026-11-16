#############################################################
## ElectricityArmorStaticShield.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		At	Origin
		Type	local
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFx	Powers\ElectricityControl\ElectricityArmorStaticShieldChild.fx
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		Sound static2_loop 70 70 0.54
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 290
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		At	Origin
		Type	local
		Bhvr	Behaviors\GenericParticleFade.bhvr
		ChildFx	Powers\ElectricityControl\ElectricityArmorStaticShieldChild.fx
		Sound oreGlow_loop 40 40 .33
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		At	Origin
		Type	SetLight
		BHVR	GENERIC\LowGlow\lightpulse3.bhvr
		LifeSpan 30
	End
End

Condition
	On	Cycle
	Time	40

	Event
		At	Origin
		Type	SetLight
		BHVR	GENERIC\LowGlow\BurnoutFade.bhvr
		LifeSpan 30
	End
End

#############################################################

End