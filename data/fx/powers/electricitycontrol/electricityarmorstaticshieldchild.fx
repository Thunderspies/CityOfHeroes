#############################################################
## ElectricityArmorStaticShieldChild.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	LlegR
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	UlegL
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	LlegL
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	LlegL
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	FootL
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	LlegR
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	UlegR
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	FootR
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
		Pmagnet	LlegR
	End

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	Powers\ElectricityControl\LeviathanEmber.part
		Part	Powers\ElectricityControl\TendrilEnergy.part
	End
End

#############################################################

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		#Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		#Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		#Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		#Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		#Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5

	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1


	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzapsRed.part
		#Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

###############################################################################

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5

	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1


	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Part	Powers\ElectricityControl\electricityzapsRed.part
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.1

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\ElectricityControl\electricityzaps.part
		Part	Powers\ElectricityControl\electricityLight.part
		LifeSpan 5
	End
End

#############################################################
End
