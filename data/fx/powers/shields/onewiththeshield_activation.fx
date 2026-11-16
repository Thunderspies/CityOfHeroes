#############################################################
## OneWithTheShield_Activation.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:OneWithTheShield_Rays1.part
		Part	:OneWithTheShield_Rays1.part
		Lifespan 35
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	Root
		ChildFX	:Child_ChestToLArmPulse.fx
		Lifespan 30
	End
End

#############################################################

Condition
	On	Time
	Time	45

	Event
		Ename	BodyAura_Initial
		Type	Local
		At	Origin
		ChildFX	:Shields_Continuing_Pulse.fx
		Lifespan 60
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound one_with_the_shield 60 60 .85
	End
End
	
#########################################################

End